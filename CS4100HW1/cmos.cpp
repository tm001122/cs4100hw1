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
#include <algorithm>
using namespace std;

vector<long long> hasher(vector<string> &v);

vector<string> divider(string s, int k);

vector <pair<int, int>> Rollingwindow(vector<long long> &v, int k);

double calculateSimilarity(vector <pair<int, int>> &v1, vector <pair<int, int>> &v2);

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        cout << "Please enter one filenames" << endl;
        return 1;
    }

    ifstream file1(argv[1]);

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
    int k = 5;

    while (!file1.eof())
    {
        vector<string> tokens1;
        getline(file1, line);
        stringstream ss(line);
        while (ss >> token)
        {
            tokens1.push_back(token);
        }

        string tokenString = "";
        for (unsigned int i = 0; i < tokens1.size(); i++)
        {
            tokenString += tokens1[i];
        }

       vector <string> dividedTokens = divider(tokenString, k);

        for (unsigned int i = 0; i < dividedTokens.size(); i++)
        {
            cout << dividedTokens[i] << endl;
        }   


        vector<long long> m1 = hasher(tokens1);
        vector <pair<int, int>> fingerPrints = Rollingwindow(m1, k);


        for (unsigned int i = 0; i < fingerPrints.size(); i++)
        {
            cout << fingerPrints[i].first << " " << fingerPrints[i].second << endl;
        }

        allFingerprints.push_back(fingerPrints);

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
    
    cout << "Similarity: " << calculateSimilarity(allFingerprints[0], allFingerprints[1]) << endl;



    return 0;
}

//can we use built in hash function
//change
vector<long long> hasher(vector<string> &v)
{
    vector<long long> hashValues;
    for (unsigned int i = 0; i < v.size(); i++)
    {
        long long hashValue = 0;
        for (unsigned int j = 0; j < v[i].length(); j++)
        {
            hashValue = (hashValue * 1234567 + v[i][j]) % 10000000000;
        }
        hashValues.push_back(hashValue);
    }

    return hashValues;

}


vector<string> divider(string s, int k)
{
    vector <string> v;
    unsigned int k1 = k;

    for (unsigned int i = 0; i < s.length(); i++)
    {
        if (s.length() - i >= k1)
            v.push_back(s.substr(i, k));
    }

    return v;
}

//should we save the index of the fingerprint or just keep track of the og file
vector <pair<int, int>> Rollingwindow(vector<long long> &v, int k)
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

    vector <pair<int, int>> finalFingerprints;
    for (unsigned int i = 0; i < selectedFingerprints.size(); i++)
    {
        if (selectedFingerprints[i].second != -1)
        {
            finalFingerprints.push_back(selectedFingerprints[i]);
        }
    }
    return finalFingerprints;
}


//if we keep track of index, does that impact similarity 
// doesnt care about duplicates 
double calculateSimilarity(vector <pair<int, int>> &v1, vector <pair<int, int>> &v2)
{
    double sharedCount = 0.0;

    for (unsigned int i = 0; i < v1.size(); i++)
    {
        sharedCount += std::count(v2.begin(), v2.end(), v1[i]);
    }
    return sharedCount / v1.size();
}