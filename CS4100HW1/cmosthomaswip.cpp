//****************************************************************
// File:    cmos.cpp
// Purpose: to divide up tokenized code files into k-grams,
//          hash those k-grams, and then use a rolling window
//          to create a set of fingerprints which are then used 
//          to determine the similarity of two files.
// Author:  Jacob Whited and Thomas 
//****************************************************************


#include <sstream>
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
    if (argc != 2)
    {
        cout << "Please enter one filenames" << endl;
        return 1;
    }

    ifstream file1(argv[1]);
    ifstream file2(argv[2]);

    if (!file1.is_open())
    {
        cout << "One or both of the files could not be opened" << endl;
        return 1;
    }



    string s1;
    string s2;
    //getline(file1, s1);
    //getline(file2, s2);
    string token;
    string line;
    vector <vector<pair<int, int>>> allFingerprints;
    int k = 3;

    while (!file1.eof())
    {
        vector<string> tokens1;
        getline(file1, line);
        stringstream ss(line);
        while (ss >> token)
        {
            tokens1.push_back(token);
        }

        for (unsigned int i = 0; i < tokens1.size(); i++)
        {
            cout << tokens1[i] << " ";
        }


        vector<int> m1 = hasher(tokens1);
        vector <pair<int, int>> result1 = Rollingwindow(m1, k);

        vector <pair<int, int>> fingerPrints1;

        for (unsigned int i = 0; i < result1.size(); i++)
        {
            if (result1[i].second != -1)
            {
                fingerPrints1.push_back(result1[i]);
            }
        }

        for (unsigned int i = 0; i < fingerPrints1.size(); i++)
        {
            cout << fingerPrints1[i].first << " " << fingerPrints1[i].second << endl;
        }

        allFingerprints.push_back(fingerPrints1);

        cout << endl;
        cout << endl;
        cout << endl;

    }
    file1.close();

    for (unsigned int i = 0; i < allFingerprints.size(); i++)
    {
        for (unsigned int j = 0; j < allFingerprints[i].size(); j++)
        {
            cout << allFingerprints[i][j].first << " " << allFingerprints[i][j].second << endl;
        }
        cout << endl;
    }



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
double calculateSimilarity(vector<pair<int, int>>& v1, vector<pair<int, int>>& v2)
{
    unordered_set<int> set1, set2;

    // Create sets of the first elements of each pair in v1
    for (const auto& p : v1)
    {
        set1.insert(p.first);
    }

    // Create sets of the first elements of each pair in v2
    for (const auto& p : v2)
    {
        set2.insert(p.first);
    }

    // Calculate the intersection size
    int intersectionSize = 0;
    for (const auto& element : set1)
    {
        if (set2.count(element) > 0)
        {
            intersectionSize++;
        }
    }

    // Calculate the union size
    // int unionSize = set1.size() + set2.size() - intersectionSize;

    // Calculate the Jaccard similarity coefficient
    double similarity = static_cast<double>(intersectionSize) / set1.size();

    return similarity;
}