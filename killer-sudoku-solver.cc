#include <iostream>
#include <unordered_map>
#include <bitset>
#include <string>
#include <set>
#include <vector>

using namespace std;

#define N 9
#define rootN 3

vector<string> cross(vector<string> A, vector<string> B)
{
	vector<string> V;
	for(auto a: A)
		for(auto b: B)
			V.push_back(a+":"+b);
	return V;
}

pair<int, int> string_to_coord(string s)
{
	auto x = s.find(':');
	return make_pair(stoi(s.substr(0, x)),
					 stoi(s.substr(x+1, s.length()-x)));
}

string coord_to_string(pair<int,int> p)
{
	return to_string(p.first)+':'+to_string(p.second);
}


int main()
{
	vector<string> places;
	for(int i=0;i<N;i++)
	{
		places.push_back(to_string(i));
	}

	vector<string> squares = cross(places, places);
	vector<vector<string>> unitlist;

	unordered_map<string, set<string>> peers;
	unordered_map<string, vector<vector<string>>> units;

	for(auto s: squares)
	{
		pair<int, int> rc = string_to_coord(s);
		vector<string> row(1, to_string(rc.first)), 
					   col(1, to_string(rc.second)), 
					   box;
		units[s].push_back(cross(row, places));
		units[s].push_back(cross(places, col));
		for(int i=0;i<rootN;i++)
		{
			for(int j=0;j<rootN;j++)
			{
				box.push_back(coord_to_string(make_pair(rootN*(rc.first/rootN) + i, 
														rootN*(rc.second/rootN) + j)));
			}
		}
		units[s].push_back(box);
		for(auto u: units[s])
		{
			for(auto s2: u)
			{
				if(s2 == s) continue;
				peers[s].insert(s2);
			}
		}
	}

	for(auto s: squares) 
	{
		cout << "SQUARE : " << s << "\n" 
			 << "UNITS : " << "\n";
		for(auto u: units[s])
		{
			for(auto s2: u)
			{
				cout << s2 << " ";
			}
			cout << "\n";
		}
		cout << "PEERS : " << "\n";
		for(auto p: peers[s])
		{
			cout << p << " ";
		}
		cout << "\n";
	}
}
