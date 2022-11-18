#include <bits/stdc++.h>


using namespace std;
set<string> good;
map<string, set<string>> first, follow;
struct GrammarSymbol{
    string name;
    int Type;
};
struct Production{
    int indexId;
    int bodySize;
    vector<GrammarSymbol> bodySymbolTable;
};

struct ItemSet{
    int indexId;
    bool flag;
    vector<ItemSet> nxt;
};
struct LR0Item{
    int productionId;
    int dotPosition;
    int type;
};
struct ActionCell{
    int stateId;
    string terminalSymbolName;
    char actionType;
    int id;
};

struct GotoCell{
    int stateId;
    string nonTerminalSymbolName;
    int nextStateId;
};
struct ProductionInfo{
    int indexId;
    string headName;
    int bodySize;
};
/**
 * 判断一个vector<string>里面是否含有左递归的产生式
 * @param production
 * @return
 */
bool IF_LEFT_RECURSIVE(string left, vector<string>& production) {
    bool ok = false;
    for (string x : production) {
        ok |= x[0] == left[0];
    }
    return ok;
}
void transform(map<string, vector<string>> S, vector<string>& production, string start) {
    set<string> s1, s2;
    for(const auto& x : production){
        for(const auto& y : S[x]){
            if(y == start) s1.insert(y);
            else s2.insert(y);
        }
    }
    production.clear();
    if(s1.size() >= s2.size()) {
        for(const auto& x : s1) production.push_back(x);
    }
    else{
        for(const auto& x : s2) production.push_back(x);
    }
}
void eliminate(string left, vector<string>& production) {
    //消除直接左递归代码
    vector<string> include; //包含左递归的产生式子
    vector<string> not_include; //不包含左递归的产生式
    vector<string> ans;
    for (string x : production) {
        if (x[0] == left[0]) {
            include.push_back(x.substr(1));
        }
        else {
            not_include.push_back(x);
        }
    }
    map<string, vector<string>> mp;
    cout << "original" << '\n';
    for (const string& x : production) {
        cout << left << "->" << x << '\n';
    }
    for (const string& x : not_include) {
        mp[left].push_back(x + left + "'");
    }
    for (const string& x : include) {
        mp[left + "'"].push_back(x + left + "'");
    }

    for (auto [str, res] : mp) {
        for(auto &x : res) {
            cout << str << "->" << x << '\n';
        }
    }

}
bool IF_EXIST_LEFT_COMMON_FACTOR(string left, vector<string> production) {

    map<char, int> cnt;
    for (string &x : production) {
        cnt[x[0]] += 1;
    }
    return (int)cnt.size() >= 2;
}
void GET_LEFT_COMMON_FACTOR(string left, vector<string> production) {

    map<char, vector<string>> str;
    for (string &x : production) {
        if(x.size() == 0) {
            str[x[0]].push_back("ε");
        }
        else{
            str[x[0]].push_back(x.substr(1));
        }

    }
    vector<string> t;
    char p;
    for(auto x : production) {
        if(str[x[0]].size() >= 2){
            p = x[0];
            continue;
        }
        t.push_back(x);
    }
    string ret;
    for(auto& x : t) {
        ret += x + "|";
    }

    ret += p;
    ret += "X";
    cout << left << "->" << ret << endl;

    for (auto [x, y] : str) {

        if(y.size() <= 1) continue;
        string t;
        t += "X";
        t += "->";
        for(auto z : y) {
            t += z + "|";
        }
        t.pop_back();
        cout << t << '\n';

    }

}
bool isTerminate(char c) {
    return c >= 'a' && c <= 'z';
}

bool isTerminate(string c) {
    return c[0] >= 'a' && c[0] <= 'z' || c[0] == '*' || c[0] == '+' || c[0] == '-' ||
    c[0] == '/' || c[0] == '(' || c[0] == ')' || c == "@";
}
vector<char> GET_PRODUCTION_FIRST(vector<string> prodution) {
    vector<char> ans;
    for(auto x : prodution) {
        if(isTerminate(x[0])) ans.push_back(x[0]);
    }
    return ans;
}

bool check(vector<string> A){
    bool ok = true;
    for(auto x : A){
        for(auto y : first[x]){
            for(auto z : follow[x]){
                ok &= y == z;
            }
        }
    }
    return ok;
}

void GET_TERMINATE_FOLLOW(string start, map<string, vector<string>> Grammar,vector<string> order){


    follow[start].insert("$");
//    cout << "order" << endl;
//    for(auto x : order) {
//        cout << x << " ";
//    }
//    cout << endl;
    auto merge_follow = [&](string x, string y, bool flag) {
        for(auto z : follow[y]) {
            if(flag && z == "@") continue;
            follow[x].insert(z);
        }
    };
    auto merge_first = [&](string x, string y, bool flag) {
        for(auto z : first[y]) {
            if(flag && z == "@") continue;
            follow[x].insert(z);
        }
    };
    for(auto l : order) {
        for(auto [x, y] :Grammar) {
            for(auto z : y){
                if(z.find(l) != -1) {
//                    cout << z << endl;
                    int pos = z.find(l);
                    int ok = 0;
                    if(pos + 1 < z.size()) {
                        ok = 1;
                    }

                    if(ok) {
                        string t = z.substr(pos + 1, 1);
                        if(isTerminate(t)) {
                            follow[l].insert(t);
                        }
                        else{
                            merge_first(l, t, true);
                        }
                    }

                    merge_follow(l, x, false);



                }
            }
        }
    }

    for(auto [x, y] : follow) {
        cout << "follow (" << x << ") : ";
        for(auto z : y) {
            cout << z << " ";
        }
        cout << endl;
    }
    cout << endl;

}
void print_info(vector<string> input, vector<string> order, map<string, vector<string>> Grammar){
    map<string, map<string,string>> S;
    for(auto x : order) {
        for(auto y : input){
            if(not first[x].count(y)) continue;
            S[x][y] = x + "->" + Grammar[x][0];

        }
        for(auto y : input) {
            if(first[x].count("@") and follow[x].count(y)) {
                S[x][y] = x + "->" + "epsilon";
            }
        }


    }
    cout << "\t";
    for(auto x : input) {
        cout << x << "\t\t";
    }
    cout << endl;
    for(auto [ele, x] : S) {
        cout << ele << "\t\t";
        if(x.size() == 0) {
            cout << "\t\t";
            continue;
        }
        for(auto [_, result] : x){
            cout << result << "\t\t";
        }
        cout << endl;
    }
}
void GET_TERMINATE_FIRST(string start, map<string, vector<string>> Grammar) {
    map<string, vector<string>> G;
    for (auto [start, ne] : Grammar) {
        for (auto neighbor : ne) {
            G[start].push_back(neighbor.substr(0,1));
        }
    }
//    for(auto [x, y] : G) {
//        cout << x << " ";
//        for(auto z : y) {
//            cout << z << " ";
//        }
//        cout << endl;
//    }

    map<string, int> vis;
    function<void(string)> DFS = [&](string u) {
        if(isTerminate(u)) {
            first[u].insert(u);
            return;
        }

        for(auto x : G[u]) {
            if(isTerminate(x)) {
                first[u].insert(x);
            }
            else {
                DFS(x);
                for (auto y : first[x]) {
                    first[u].insert(y);
                }
            }
        }
        vis[u] = 1;
    };
    for(auto [x, y] :G){
        if(!vis[x]){
            vis[x] = 1;
            DFS(x);
        }
    }

    for(auto [x, y] : first) {
        cout << "first (" << x << ") : ";
        for(auto z : y) {
            cout << ((z == "@") ? ("epsilon") : z) << " ";
        }
        cout << endl;
    }
    cout << endl;
}
void init(map<string, vector<string>>& Grammar){

    Grammar["E"] = {"TM"};
    Grammar["M"] = {"+TM", "@"};
    Grammar["T"] = {"FN"};
    Grammar["N"] = {"*FN", "@"};
    Grammar["F"] = {"(E)", "a"};

}

void getClosure(ItemSet start, vector<ItemSet>& path){
    //对于核心项的项目闭包，我们只需要利用深度优先收缩不断将未扩展的产生式添加到项目集合即可
    path.push_back(start);
    for(auto x : start.nxt){
        getClosure(x, path);
    }
}
vector<pair<int,bool>> ID;
void exhaustTransition(ItemSet x) {

    ID.push_back({x.indexId, x.flag == true});
    for(auto neighbor : x.nxt){
        exhaustTransition(x);
    }

}


int main() {


    vector<string> production = {"Aa", "Ab", "Ac", "d", "e", "f", "ε"};
    vector<string> production1 = {"αβ1", "αβ2", "αβ3", "ε"};
    vector<string> production2 = {"Mid", "TME", "-E", "T-id"};
//    cout << "exist!!!" << endl;
//    printf("存在左递归!!\n");

//    if(IF_LEFT_RECURSIVE("A", production)){
//        cout << "exist!!";
//    }
//    else{
//        cout << "not exist!!";
//    }
//    cout << endl;
//    eliminate("A", production);
//    bool ok = IF_EXIST_LEFT_COMMON_FACTOR("A", production1);
////    cout << ok << '\n';
//    GET_LEFT_COMMON_FACTOR("S",production2);
    map<string, vector<string>> Grammar;
    init(Grammar);
    GET_TERMINATE_FIRST("E", Grammar);
    GET_TERMINATE_FOLLOW("E", Grammar, {"E", "M", "T", "N", "F"});
    print_info({"a", "+", "*", "(", ")", "$"}, {"E", "M", "T", "N", "F"},Grammar);
//    if (IF_LEFT_RECURSIVE(production, {"1"})) {
//       cout << "YES" << '\n';
//    }
//    else {
//        cout << "NO" << '\n';
//    }
    return 0;
}


