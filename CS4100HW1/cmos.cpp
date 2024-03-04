//****************************************************************
// File:    cmos.cpp
// Purpose: to divide up tokenized code files into k-grams,
//          hash those k-grams, and then use a rolling window
//          to create a set of fingerprints which are then used 
//          to determine the similarity of two files.
// Author:  Jacob Whited and Thomas 
//****************************************************************



#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <limits.h>
#include <utility>
#include <fstream> 
#include <unordered_set>
using namespace std;

vector<int> hasher(vector<string> &v);

vector<string> divider(string s, int k);

vector <pair<int, int>> Rollingwindow(vector<int> &v, int k);

double calculateSimilarity(vector <pair<int, int>> &v1, vector <pair<int, int>> &v2);

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        cout << "Please enter two filenames" << endl;
        return 1;
    }

    ifstream file1(argv[1]);
    ifstream file2(argv[2]);

    if (!file1.is_open() || !file2.is_open())
    {
        cout << "One or both of the files could not be opened" << endl;
        return 1;
    }



    string s1;
    string s2;
    //getline(file1, s1);
    //getline(file2, s2);
    vector<string> tokens1;
    string token;
    while (file1 >> token)
    {
        tokens1.push_back(token);
    }
    vector<string> tokens2;
    while (file2 >> token)
    {
        tokens2.push_back(token);
    }
    
    file1.close();
    file2.close();
    int k = 3;

    for (unsigned int i = 0; i < tokens1.size(); i++)
    {
        cout << tokens1[i] << endl;
    }

    cout << endl;

    for (unsigned int i = 0; i < tokens2.size(); i++)
    {
        cout << tokens2[i] << endl;
    }


    vector<int> m1 = hasher(tokens1);
    vector <pair<int, int>> result1 = Rollingwindow(m1, 4);

    vector<int> m2 = hasher(tokens2);
    vector <pair<int, int>> result2 = Rollingwindow(m2, 4);

    vector <pair<int, int>> fingerPrints1;
    vector <pair<int, int>> fingerPrints2;

    for (unsigned int i = 0; i < result1.size(); i++)
    {
       if (result1[i].second != -1)
       {
           fingerPrints1.push_back(result1[i]);
       }
    }

    for (unsigned int i = 0; i < result2.size(); i++)
    {
        if (result2[i].second != -1)
        {
            fingerPrints2.push_back(result2[i]);
        }
    }

    for (unsigned int i = 0; i < fingerPrints1.size(); i++)
    {
        cout << fingerPrints1[i].first << " " << fingerPrints1[i].second << endl;
    }

    cout << endl;
    cout << endl;

    for (unsigned int i = 0; i < fingerPrints2.size(); i++)
    {
        cout << fingerPrints2[i].first << " " << fingerPrints2[i].second << endl;
    }

    double similarity = calculateSimilarity(fingerPrints1, fingerPrints2);
    cout << "The similarity of the two files is: " << similarity * 100 << "%" << endl;


    return 0;

    

}

//
vector<int> hasher(vector<string> &v)
{
    std::hash<std::string> str_hash;
    vector<int> hashes;
    for (unsigned int i = 0; i < v.size(); i++)
    {
        hashes.push_back(str_hash(v[i]));
    }
    return hashes;
}


vector<string> divider(string s, int k)
{
    vector <string> v;

    for (unsigned int i = 0; i < s.length(); i++)
    {
        v.push_back(s.substr(i, k));
    }

    return v;
}

vector <pair<int, int>> Rollingwindow(vector<int> &v, int k)
{
    vector <pair<int, int>> selectedFingerprints;
    for (unsigned int i = 0; i < v.size(); i++)
    {
        selectedFingerprints.push_back(make_pair(v[i], -1));
    }
    int minValue = INT_MAX;
    int minIndex = -1;
    unsigned int count = k - 1;
    int start = 0;

    while (count < v.size())
    {
        for (unsigned int i = start; i <= count; i++)
        {
            if (v[i] < minValue)
            {
                minValue = v[i];
                minIndex = i;
            }
        }
        selectedFingerprints[minIndex].first = minValue;
        selectedFingerprints[minIndex].second = minIndex;
        minValue = INT_MAX;
        start++;
        count++;
    }

    return selectedFingerprints;
}

//
double calculateSimilarity(vector <pair<int, int>> &v1, vector <pair<int, int>> &v2)
{

}