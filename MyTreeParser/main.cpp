//#pragma once
//This calc based on tree class without brackets
#include <iostream>
#include <vector>
using namespace std;


#include "TreeNode.h"
#include "OperationsTree.h"

int main()
{
		vector <TreeNode*> vectExample;
		string str;
		cin >> str;
		makeNodesVector(str, vectExample);
		makeConnectedTree(vectExample);
		cout << "Result is " << calculateTree(vectExample) << endl;
	return 0;
}