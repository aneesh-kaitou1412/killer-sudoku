#include <iostream>
#include <unordered_map>
#include <bitset>
#include <string>
#include <set>
#include <vector>

using namespace std;

#define N 9
#define rootN 3

struct Cage
{
	int sum, m, M;
	vector<string> cells;

	Cage(int s, vector<string> v)
	{
		this->sum = s;
		this->cells = v;
		int n = v.size()-1;
		this->m = this->sum - n*N + ((n*(n-1))/2);
		this->M = this->sum - ((n*(n+1))/2);
		this->m = this->m > 1 ? this->m : 1;
		this->M = this->M < N ? this->M : N;
	}

	void print()
	{
		cout << "Sum : " << this->sum << " Cells : " << cells.size() << "\n";
		cout << "Max : " << this->m << " Min : " << this->M << "\n";		
	}
};

vector<string> places;
vector<string> squares;

unordered_map<string, set<string>> peers;
unordered_map<string, vector<vector<string>>> units;
unordered_map<string, Cage*> cages;


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

bool assign(unordered_map<string, bitset<N>>& values, string s, int d);
bool eliminate(unordered_map<string, bitset<N>>& values, string s, int d);

bool assign(unordered_map<string, bitset<N>>& values, string s, int d)
{
	// cout << "Assign " << s << " " << d << "\n"; 
	int cursum = d;
	for(auto c: cages[s]->cells)
	{
		if(c == s) continue;
		if(values[c].count() == 1)
		{
			for(int i=0;i<N;i++)
			{
				if(values[c][i]==1)
				{
					cursum += i+1;
					break;
				}
			}
		}
	}

	if(cursum > cages[s]->sum) return false;
	// cout << "Sum OK" << "\n";

	for(int i=0;i<N;i++)
	{
		if(i == d-1) continue;
		else
		{
			bool b = eliminate(values, s, i+1);
			if (!b) return false;
		}
	}
	return true;
}

bool eliminate(unordered_map<string, bitset<N>>& values, string s, int d)
{
	// cout << "Eliminate " << s << " " << d << "\n";
	if(values[s][d-1] == 0) return true;
	values[s].reset(d-1);

	if(values[s].count() == 0) return false;

	if(values[s].count() == 1)
	{
		int d2;
		for(int i=0;i<N;i++)
		{
			if(values[s][i]==1)
			{
				d2=i+1;
				break;
			}
		}
		// cout << d2 << "\n";
		for(auto s2: peers[s])
		{
			bool b = eliminate(values, s2, d2);
			if (!b) return false;
		}
	}

	for(auto u: units[s])
	{
		// for(auto s2: u)
		// {
		// 	cout << s2 << " ";
		// }
		// cout << "\n";

		vector<string> dplaces;
		for(auto s2: u)
			if(values[s2][d-1] == 1)
				dplaces.push_back(s2);

		if(dplaces.size() == 0) return false;
		if(dplaces.size() == 1)
		{
			bool b = assign(values, dplaces[0], d);
			if(!b) return false;
		}
	}

	return true;
}

pair<bool, unordered_map<string, bitset<N>>> solve(bool b, unordered_map<string, bitset<N>> values)
{
	if(!b) return make_pair(false, values);

	// check if all cells filled
	bool x = true;
	for(auto v: values)
	{
		if(v.second.count() == 1) continue;
		x = false;
		break;
	}
	if(x) return make_pair(true, values);

	// assign a value and continue
	string smin; int n=N;
	for(auto s: squares)
	{
		if(values[s].count() < n && values[s].count() > 1)
		{
			smin = s;
			n=values[s].count();
		}
	}

	for(int i=0;i<N;i++)
	{
		if(values[smin][i] == 1)
		{
			unordered_map<string, bitset<N>> nextvalues = values;
			bool y = assign(nextvalues, smin, i+1);
			if(!y) continue;
			auto res = solve(true, nextvalues);
			if(res.first)
				return res;
		}
	}
	return make_pair(false, values);
}

int main()
{
	for(int i=0;i<N;i++)
	{
		places.push_back(to_string(i));
	}
	squares = cross(places, places);

	int n, c, s, x, y;
	cin >> n;
	for(int i=0;i<n;i++)
	{
		cin >> c >> s;
		vector<string> v;
		for(int j=0;j<c;j++)
		{
			cin >> x >> y;
			v.push_back(coord_to_string(make_pair(x, y)));
		}
		Cage* cage = new Cage(s, v);
		for(auto c: cage->cells)
		{
			cages[c] = cage;
		}
	}

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

	/* Print to check */
	// for(auto s: squares) 
	// {
	// 	cout << "SQUARE : " << s << "\n" 
	// 		 << "UNITS : " << "\n";
	// 	for(auto u: units[s])
	// 	{
	// 		for(auto s2: u)
	// 		{
	// 			cout << s2 << " ";
	// 		}
	// 		cout << "\n";
	// 	}
	// 	cout << "CAGE : " << "\n";
	// 	cages[s]->print();
	// 	for(auto c: cages[s]->cells)
	// 	{
	// 		cout << c << " ";
	// 	}
	// 	cout << "\n";
	// 	cout << "PEERS : " << "\n";
	// 	for(auto p: peers[s])
	// 	{
	// 		cout << p << " ";
	// 	}
	// 	cout << "\n\n";
	// }

	unordered_map<string, bitset<N>> values;

	for(auto s: squares)
	{
		bitset<N> b;
		b.reset();
		for(int i=cages[s]->m; i<=cages[s]->M; i++)
		{
			b.set(i-1);
		}
		values[s] = b;
	}

	auto res = solve(true, values);
	cout << res.first << "\n";
	values = res.second;
	/* check bitsets */
	for(int i=0;i<N;i++)
	{
		for(int j=0;j<N;j++)
		{
			if(j%3 == 0)
				cout << " | ";
			string c = coord_to_string(make_pair(i, j));
			for(int k=0;k<N;k++)
			{
				if(values[c][k]==1)
				{
					cout << k+1 << ",";
				}
			}
			cout << " ";
		}
		cout << "\n";
	}
}
