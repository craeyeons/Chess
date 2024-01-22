#pragma once
#include <bits/stdc++.h>

using namespace std;

pair<int, int> convertPoisition(string position) {
    int col = position[0] - 'a';
    int row = position[1] - '1';

    return make_pair(row, col);
}

string convertPosition(pair<int, int> position) {
    string col = string(1, position.second + 'a');
    string row = string(1, position.first + '1');

    return col + row;
}
