#include <bits/stdc++.h>

using namespace std;
using ll = long long;
using ld = long double;
#define endl "\n"

struct Monster {
    int x, y, hp, gold, exp;
};

string s; // take input
vector<Monster> monsters;
vector<pair<char, pair<int, int>>> cur_res, res;
int bs, bp, br, lsc, lpc, lrc; // hero
int sx, sy, w, h, nt; // start_x, start_y, width, height, num_turns
int n;

// differentiate between range and speed

int eval(vector<int> route) {
    int xp = 0, level = 0, threshold = 1000;
    int cx = sx, cy = sy;
    int pnt = 0, ret = 0, cur_health = monsters[route[0]].hp;
    for(int i=0; i<nt; i++) {
        // add 1e6 to avoid precision issues when rounding down
        int cs = (double)bs*((double)1+(double)level*((double)lsc/(double)100))+1e-6, cp = (double)bp*((double)1+(double)level*((double)lpc/(double)100))+1e-6, cr = (double)br*((double)1+(double)level*((double)lrc/(double)100))+1e-6;
        if(pow(cr, 2)<pow(cx-monsters[route[pnt]].x, 2)+pow(cy-monsters[route[pnt]].y, 2)) {
            int tx = cx-cs, ty = cy, tv = 0; 
            int td = pow(tx-monsters[route[pnt]].x, 2)+pow(ty-monsters[route[pnt]].y, 2);
            int copy_tx = tx;
            for(int j=copy_tx; j<=copy_tx+2*cs; j++) {
                if(j<0 or j>w) continue;
                while(j<=copy_tx+cs and pow(j-cx, 2)+pow(tv+1, 2)<=pow(cs, 2)) tv++;
                while(j>copy_tx+cs and pow(j-cx, 2)+pow(tv, 2)>pow(cs,2)) tv--;
                int tmp1 = pow(monsters[route[pnt]].x-j, 2)+pow(monsters[route[pnt]].y-(cy+tv), 2);
                int tmp2 = pow(monsters[route[pnt]].x-j, 2)+pow(monsters[route[pnt]].y-(cy-tv), 2);
                if(cy+tv<=h and tmp1<td) {
                    td = tmp1;
                    tx = j;
                    ty = cy+tv;
                }
                if(cy-tv>=0 and tmp2<td) {
                    td = tmp2;
                    tx = j;
                    ty = cy-tv;
                }
            }
            cx = tx;
            cy = ty;
            cur_res.push_back({'m', {cx, cy}});
        }
        else {
            cur_res.push_back({'a', {route[pnt], -1}});
            cur_health -= cp;
            if(cur_health<=0) {
                ret += monsters[route[pnt]].gold;
                xp += monsters[route[pnt]].exp;
                while(xp>=threshold) {
                    level++;
                    threshold += 1000 + level*(level+1)*50;
                }
                pnt++;
                if(pnt==n) break;
                cur_health = monsters[route[pnt]].hp;
            }
        }
    }
    return ret;
}

ld P(int cur, int next, int t) {
    if(next>cur) return 1;
    return exp((next-cur)/(t));
}

int sol;
mt19937 gen(random_device{}());
uniform_real_distribution<> dis(0.0, 1.0);

void solve() {
    uniform_int_distribution<> din(0, n-1);
    vector<int> route(n), next_route(n);
    for(int i=0; i<n; i++) route[i] = i;
    int cur = eval(route),  next;
    auto now = chrono::steady_clock::now;
    auto start = now();
    auto elapsed = now()-start;
    ld t = 1e3;
    while(elapsed<20000s) {
        t *= 0.9999; // make hihger and add more time
        if(t<1) {
            cout << "line 127 " << elapsed.count()/1e9 << endl;
            break;
        }
        int f = din(gen), s = din(gen);
        while(f==s) s = din(gen);
        if(f>s) swap(f, s);
        next_route = route;
        for(int i=f; i<(s+f)/2; i++) swap(next_route[i], next_route[s-i+f]);
        
        if(cur>sol) {
            sol = cur;
            res = cur_res;
        }
        cur_res.clear();
        next = eval(next_route);

        //cout << t << " " << P(cur, next, t) << " " << next << " " << sol << endl;

        if(P(cur, next, t)>=dis(gen)) {
            route = next_route;
            cur = next;
        }
        elapsed = now()-start;
    }
    cout << sol << endl;
}

int main() {
    ifstream fin("inputs/002.json");
    ofstream fout("output.json");

    ios::sync_with_stdio(0);
    cin.tie(0);
    
    Monster temp_monster; // cur monster
    while(fin>>s) {
        if(s=="\"base_speed\":") {
            fin >> s;
            bs = stoi(s.substr(0, s.size()-1));
        }
        else if(s=="\"base_power\":") {
            fin >> s;
            bp = stoi(s.substr(0, s.size()-1));
        }
        else if(s=="\"base_range\":") {
            fin >> s;
            br = stoi(s.substr(0, s.size()-1));
        }
        else if(s=="\"level_speed_coeff\":") {
            fin >> s;
            lsc = stoi(s.substr(0, s.size()-1));
        }
        else if(s=="\"level_power_coeff\":") {
            fin >> s;
            lpc = stoi(s.substr(0, s.size()-1));
        }
        else if(s=="\"level_range_coeff\":") {
            fin >> s;
            lrc = stoi(s);
        }
        else if(s=="\"start_x\":") {
            fin >> s;
            sx = stoi(s.substr(0, s.size()-1));
        }
        else if(s=="\"start_y\":") {
            fin >> s,
            sy = stoi(s.substr(0, s.size()-1));
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
            nt = stoi(s.substr(0, s.size()-1)); 
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
            temp_monster.gold = stoi(s.substr(0, s.size()-1));
        }
        else if(s=="\"exp\":") {
            fin >> s;
            temp_monster.exp = stoi(s);
            monsters.push_back(temp_monster);
        }
    }
    
    n = monsters.size();

    for(int i=0; i<50; i++) solve();
    //solve();

    fout << "{\n\"moves\":[\n";
    for(int i=0; i<nt; i++) {
        if(res[i].first=='a') {
            fout << "{\n\"type\": \"attack\"," << endl << "\"target_id\": " << res[i].second.first << endl << "},\n";
        }
        else {
            fout << "{\n\"type\": \"move\"," << endl << "\"target_x\": " << res[i].second.first << ",\n" << "\"target_y\": " << res[i].second.second << endl << "},\n";
        }
    }
    fout << "]\n}";
}