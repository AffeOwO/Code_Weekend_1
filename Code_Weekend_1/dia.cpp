#include <iostream>
#include <vector>
#include <fstream>
#include <cassert>
#include <cmath>
using namespace std;

using namespace std;
using ll = long long;
using ld = long double;
//#define endl "\n"

int baseSpeed, basePower, baseRange, coefSpeed, coefPower, coefRange;

struct Monster {
    int x, y, hp, gold, exp;
    double dist(int x0, int y0) {
        return sqrt((x - x0) * (x - x0) + (y - y0) * (y - y0));
    }
};

double dist(int x0, int y0, int x1, int y1) {
    return sqrt((x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0));
}

vector<Monster> monsters;

struct Move {
    bool attack;
    int target;
    int x, y;
};

int maxTurns;
int startX, startY, w, h;
double hpCoeff;

pair<int, vector<Move>> calc(int targetLevel) {
    int level = 0;
    int exp = 0;
    int threshold = 1000;
    int n = monsters.size();
    int x = startX;
    int y = startY;
    int gold = 0;
    int speed = baseSpeed;
    int power = basePower;
    int range = baseRange;
    vector<bool> killed(n);
    vector<Move> moves;
    while (level < targetLevel) {
        pair<double, int> best = {-1, -1};
        for (int i = 0; i < n; ++i) {
            if (killed[i]) {
                continue;
            }
            if (monsters[i].dist(x, y) / speed > maxTurns - moves.size()) {
                continue;
            }
            // best = max(best, make_pair((double)monsters[i].exp / (monsters[i].dist(x, y) + monsters[i].hp + 1e-6), i));
            // best = max(best, make_pair((double)monsters[i].exp / ((monsters[i].dist(x, y) + 1e-6) * monsters[i].hp), i));
            best = max(best, make_pair((double)monsters[i].exp / (monsters[i].hp * hpCoeff + monsters[i].dist(x, y) * (1 - hpCoeff) + 1e-6), i));

        }
        if (best.second == -1) {
            return {gold, moves};
        }
        Monster target = monsters[best.second];
        if (target.dist(x, y) > range) {
            int dx = target.x - x;
            int dy = target.y - y;
            double len = target.dist(x, y);
            double ndx = dx / len;
            double ndy = dy / len;
            int newX = x + ndx * min((double)speed, target.dist(x, y) + range * 0.9);
            int newY = y + ndy * min((double)speed, target.dist(x, y) + range * 0.9);
            if (dist(x, y, newX, newY) > speed) {
                pair<double, pair<int, int>> off = make_pair(1000000000, make_pair(-2, -2));
                for (int xoff = -1; xoff <= 1; ++xoff) {
                    for (int yoff = -1; yoff <= 1; ++yoff) {
                        if (dist(x, y, newX + xoff, newY + yoff) <= speed) {
                            off = min(off, make_pair(target.dist(newX + xoff, newY + yoff), make_pair(xoff, yoff)));
                        }
                    }
                }
                assert(off.second.first != -2);
                newX += off.second.first;
                newY += off.second.second;
            }
            x = max(0, newX);
            y = max(0, newY);
            x = min(x, w);
            y = min(y, h);
            Move move_;
            move_.attack = false;
            move_.x = x;
            move_.y = y;
            moves.push_back(move_);
            if (moves.size() == maxTurns) {
                return {gold, moves};
            }
        }
        if (target.dist(x, y) > range) {
            continue;
        }
        assert(target.dist(x, y) <= range);
        while (target.hp > 0) {
            target.hp -= power;
            Move move_;
            move_.attack = true;
            move_.target = best.second;
            moves.push_back(move_);
            if (moves.size() == maxTurns) {
                return {gold, moves};
            }
        }
        killed[best.second] = true;
        gold += target.gold;
        exp += target.exp;
        while (exp >= threshold) {
            ++level;
            threshold += 1000 + (level + 1) * (level) * 50;
        }
        speed = baseSpeed * (1 + level * coefSpeed / 100.0);
        power = basePower * (1 + level * coefPower / 100.0);
        range = baseRange * (1 + level * coefRange / 100.0);
    }
    while (moves.size() < maxTurns) {
        pair<double, int> best = {-1, -1};
        for (int i = 0; i < n; ++i) {
            if (killed[i]) {
                continue;
            }
            if (monsters[i].dist(x, y) / speed > maxTurns - moves.size()) {
                continue;
            }
            // best = max(best, make_pair((double)monsters[i].gold / (monsters[i].dist(x, y) + monsters[i].hp + 1e-6), i));
            // best = max(best, make_pair((double)monsters[i].gold / ((monsters[i].dist(x, y) + 1e-6) * monsters[i].hp), i));
            best = max(best, make_pair((double)monsters[i].gold / (monsters[i].hp * hpCoeff + monsters[i].dist(x, y) * (1 - hpCoeff) + 1e-6), i));
        }
        if (best.second == -1) {
            return {gold, moves};
        }
        Monster target = monsters[best.second];
        if (target.dist(x, y) > range) {
            int dx = target.x - x;
            int dy = target.y - y;
            double len = target.dist(x, y);
            double ndx = dx / len;
            double ndy = dy / len;
            int newX = x + ndx * min((double)speed, target.dist(x, y) + range * 0.9);
            int newY = y + ndy * min((double)speed, target.dist(x, y) + range * 0.9);
            if (dist(x, y, newX, newY) > speed) {
                pair<double, pair<int, int>> off = make_pair(1000000000, make_pair(-2, -2));
                for (int xoff = -1; xoff <= 1; ++xoff) {
                    for (int yoff = -1; yoff <= 1; ++yoff) {
                        if (dist(x, y, newX + xoff, newY + yoff) <= speed) {
                            off = min(off, make_pair(target.dist(newX + xoff, newY + yoff), make_pair(xoff, yoff)));
                        }
                    }
                }
                assert(off.second.first != -2);
                newX += off.second.first;
                newY += off.second.second;
            }
            x = max(0, newX);
            y = max(0, newY);
            x = min(x, w);
            y = min(y, h);
            Move move_;
            move_.attack = false;
            move_.x = x;
            move_.y = y;
            moves.push_back(move_);
            if (moves.size() == maxTurns) {
                return {gold, moves};
            }
        }
        if (target.dist(x, y) > range) {
            continue;
        }
        assert(target.dist(x, y) <= range);
        while (target.hp > 0) {
            target.hp -= power;
            Move move_;
            move_.attack = true;
            move_.target = best.second;
            moves.push_back(move_);
            if (moves.size() == maxTurns) {
                return {gold, moves};
            }
        }
        killed[best.second] = true;
        gold += target.gold;
        exp += target.exp;
        while (exp >= threshold) {
            ++level;
            threshold += 1000 + (level + 1) * (level) * 50;
        }
        speed = baseSpeed * (1 + level * coefSpeed / 100.0);
        power = basePower * (1 + level * coefPower / 100.0);
        range = baseRange * (1 + level * coefRange / 100.0);
    }
    return {gold, moves};
}

int main() {
    ifstream fin("025.json");
    ofstream fout("output.json");

    ios::sync_with_stdio(0);
    cin.tie(0);

    string s; // take input
    Monster temp_monster; // cur monster
    int I = 0;
    while(fin>>s) {
        if(s=="\"base_speed\":") {
            fin >> s;
            baseSpeed = stoi(s.substr(0, s.size()-1));
        }
        else if(s=="\"base_power\":") {
            fin >> s;
            basePower = stoi(s.substr(0, s.size()-1));
        }
        else if(s=="\"base_range\":") {
            fin >> s;
            baseRange = stoi(s.substr(0, s.size()-1));
        }
        else if(s=="\"level_speed_coeff\":") {
            fin >> s;
            coefSpeed = stoi(s.substr(0, s.size()-1));
        }
        else if(s=="\"level_power_coeff\":") {
            fin >> s;
            coefPower = stoi(s.substr(0, s.size()-1));
        }
        else if(s=="\"level_range_coeff\":") {
            fin >> s;
            coefRange = stoi(s);
        }
        else if(s=="\"start_x\":") {
            fin >> s;
            startX = stoi(s.substr(0, s.size()-1));
        }
        else if(s=="\"start_y\":") {
            fin >> s,
            startY = stoi(s.substr(0, s.size()-1));
        }
        else if(s=="\"width\":") {
            fin >> s;
            w = stoi(s.substr(0, s.size()-1));
        }
        else if(s=="\"height\":") {
            fin >> s;
            h = stoi(s.substr(0, s.size()-1));
        }
        else if(s=="\"num_turns\":") {
            fin >> s;
            maxTurns = stoi(s.substr(0, s.size()-1));
        }
        else if(s=="\"x\":") {
            fin >> s;
            temp_monster.x = stoi(s.substr(0, s.size()-1));
        }
        else if(s=="\"y\":") {
            fin >> s;
            temp_monster.y = stoi(s.substr(0, s.size()-1));
        }
        else if(s=="\"hp\":") {
            fin >> s;
            temp_monster.hp = stoi(s.substr(0, s.size()-1));
        }
        else if(s=="\"gold\":") {
            fin >> s;
            temp_monster.gold = stoi(s.substr(0, s.size() - 1));
        }
        else if(s=="\"exp\":") {
            fin >> s;
            temp_monster.exp = stoi(s);
            monsters.push_back(temp_monster);
        }
        /*
        else if(s=="\"gold\":") {
            fin >> s;
            temp_monster.gold = stoi(s);
            monsters.push_back(temp_monster);
        }
        else if(s=="\"exp\":") {
            fin >> s;
            temp_monster.exp = stoi(s.substr(0, s.size() - 1));

        }
        */
    }

    int maxGold = -1;
    vector<Move> bestMoves(0);
    int bestTargetLevel = -1;
    const int MAX_LEVEL = 100;// 235;
    for (int targetLevel = 0; targetLevel <= MAX_LEVEL; ++targetLevel) {
        for (hpCoeff = 0.0; hpCoeff <= 1.001; hpCoeff += 0.1) {
            auto res = calc(targetLevel);
            if (maxGold < res.first) {
                maxGold = res.first;
                bestMoves = res.second;
                bestTargetLevel = targetLevel;
            }
        }
    }

    fout << "{\n";
    fout << "   \"moves\": [\n";
    for (int i = 0; i < (int)bestMoves.size(); ++i) {
        Move& move_ = bestMoves[i];
        fout << "       {\n";
        if (!move_.attack) {
            fout << "           \"type\": \"move\",\n";
            fout << "           \"target_x\": " << move_.x << ",\n";
            fout << "           \"target_y\": " << move_.y << "\n";
        }
        else {
            fout << "           \"type\": \"attack\",\n";
            fout << "           \"target_id\": " << move_.target << "\n";
        }
        if (i != (int)bestMoves.size() - 1) fout << "       },\n";
        else fout << "      }\n";
    }
    fout << "   ]\n";
    fout << "}";
    cout << maxGold << endl;
    cout << bestTargetLevel;
}
