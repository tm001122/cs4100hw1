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
#include <iomanip>
using namespace std;

vector<long long> hasher(vector<string> &v);

vector<string> divider(string s, int k);

vector <pair<long long, string>> Rollingwindow(vector<long long> &v, int k, string fileName);

double calculateSimilarity(vector <pair<long long, string>> &v1, vector <pair<long long, string>> &v2);

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
    vector <vector<pair<long long, string>>> allFingerprints;
    int k = 15;
    int w = 25;


    while (!file1.eof())
    {

        string fileName = "";
        file1 >> fileName;

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

        vector<long long> m1 = hasher(dividedTokens);

        vector <pair<long long, string>> fingerPrints = Rollingwindow(m1, w, fileName);

        allFingerprints.push_back(fingerPrints);

    }
    file1.close();

    vector <pair <double, string>> similarities;

    for (unsigned int i = 0; i < allFingerprints.size(); i++)
    {
        for (unsigned int j = i + 1; j < allFingerprints.size(); j++)
        {
            similarities.push_back(make_pair(calculateSimilarity(allFingerprints[i], allFingerprints[j]), allFingerprints[i][0].second + " and " + allFingerprints[j][0].second));
        }
    }

    sort(similarities.begin(), similarities.end());
    for (unsigned int i = 0; i < similarities.size(); i++)
    {
        cout << similarities[i].second << ":" << similarities[i].first << endl;
    }

    return 0;
}

//can we use built in hash function
//change
vector<long long> hasher(vector<string> &v)
{
    vector <long long> hashValues;
    for (unsigned int i = 0; i < v.size(); i++)
    {
        long long hashValue = 0;
        for (unsigned int j = 0; j < v[i].size(); j++)
        {
            hashValue += v[i][j] * (257 + (j + 1));
        }
        hashValue = hashValue % 734807430501;
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
vector <pair<long long, string>> Rollingwindow(vector<long long> &v, int k, string fileName)
{
    vector <pair<long long, string>> selectedFingerprints;
    for (unsigned int i = 0; i < v.size(); i++)
    {
        selectedFingerprints.push_back(make_pair(v[i], "-1"));
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
        selectedFingerprints[minIndex].second = fileName;  
        minValue = INT_MAX;
        start++;
        count++;
    }

    vector <pair<long long, string>> finalFingerprints;
    for (unsigned int i = 0; i < selectedFingerprints.size(); i++)
    {
        if (selectedFingerprints[i].second != "-1")
        {
            finalFingerprints.push_back(selectedFingerprints[i]);
        }
    }
    return finalFingerprints;
}


//if we keep track of index, does that impact similarity 
// doesnt care about duplicates 
double calculateSimilarity(vector <pair<long long, string>> &v1, vector <pair<long long, string>> &v2)
{
    unordered_set<long long> set1, set2;
    for (unsigned int i = 0; i < v1.size(); i++)
    {
        set1.insert(v1[i].first);
    }

    for (unsigned int i = 0; i < v2.size(); i++)
    {
        set2.insert(v2[i].first);
    }

    double sharedCount = 0.0;
    for (unsigned int i = 0; i < set1.size(); i++)
    {
        if (set2.count(v1[i].first) > 0)
        {
            sharedCount++;
        }
    }

    double similarity = sharedCount / set1.size();
    return similarity;
}