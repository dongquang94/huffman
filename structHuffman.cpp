#include <iostream>
#include <string>
#include <iomanip>
#include <conio.h>
#include <vector>
#include <fstream>
#include <stdlib.h>
#include <windows.h>

using namespace std;

typedef struct StructFrequency
{
	string Key;
	int Value;
} 	sFreq ;

typedef struct StructNode
{
	sFreq info;
	struct StructNode *left;
	struct StructNode *right;
}	sNode ;

typedef struct StructCode
{
	string Key;
	string Code;
} 	sCode ;

typedef sNode* NodePtr;
int Count = 0;
vector<sCode> vCode;
string strInput = "";
void Init(NodePtr *pTree){
	*pTree = NULL;
}

int IsEmpty(){
	//return (pTree == NULL ? 1 : 0);
}

NodePtr NewNode(){
	NodePtr p;
	p = new sNode;
	return p;
}

NodePtr CreateNode(sFreq s){
	NodePtr node;
	node = NewNode();
	node->info = s;
	node->left = NULL;
	node->right = NULL;
	return node;
}

void FreeNode(NodePtr p){
	free(p);
}

vector<sFreq> ReadFile(string url){
	vector<sFreq> vFreg;
	string arrStrInput[10000]; // Array data input
	//string strInput = "";  //String data input convert from arrStrInput
	string str; // temp read file
    int _count = 0, k = 1, c;
	int temp[10000], dem[255], kitu[255];

    ifstream file(url.c_str());
    if (file.is_open()){
    	while (!file.eof()){
            getline(file, str);
            arrStrInput[_count] = str;
			_count++;
        }
    }else{
		cout<<"error!";
	}
    file.close();
	strInput = "";
	// convert string[] to string
    for (int i = 0; arrStrInput[i]!="\0"; i++){
	 	strInput = strInput + arrStrInput[i];
	}
	Count = strInput.length();
	
	//convert string to char[]
    char arrCharInput[Count];
    strInput.copy(arrCharInput, Count);
	
	for(int i = 0; i < Count; i++){
	 	c = (int) arrCharInput[i];
	 	if (!temp[c]) {
	 		temp[c] = k;
	 		k++;
		}else{
			temp[c];
		}
		dem[temp[c]]++;
		kitu[temp[c]] = c;
	}
	
	//-------------------
	sFreq _sFreq;
	for (int i = 1; i < k; i++) {
		_sFreq.Key = char(kitu[i]);
		_sFreq.Value = dem[i];
		vFreg.push_back(_sFreq);
 	}
	return vFreg;
}

int StringToChar(string str){
	char ch[1];
	str.copy(ch, 1);
	int i = (int)ch[0];
	return i;
}

vector<sFreq> SortFrequency(vector<sFreq> pVFreq){
	for(int i = 0; i < pVFreq.size() - 1; i++){
		for(int k = i + 1; k < pVFreq.size(); k++){
			if(pVFreq[i].Value == pVFreq[k].Value){
				if(StringToChar(pVFreq[i].Key) > StringToChar(pVFreq[k].Key)){
					sFreq s = pVFreq[i];
					pVFreq[i] = pVFreq[k];
					pVFreq[k] = s;
				}
			}else if(pVFreq[i].Value < pVFreq[k].Value){
				sFreq s = pVFreq[i];
				pVFreq[i] = pVFreq[k];
				pVFreq[k] = s;
			}
		}
	}
	return pVFreq;
}

NodePtr BuildTree(vector<sFreq> pVFreq){
	vector<NodePtr> _vParent;
	NodePtr nodeTree, _left, _right;
	do{
		sFreq _sFreq;
		_sFreq.Value = pVFreq[pVFreq.size()-1].Value + pVFreq[pVFreq.size()-2].Value;
		_sFreq.Key = pVFreq[pVFreq.size()-1].Key + pVFreq[pVFreq.size()-2].Key;
		nodeTree = CreateNode(_sFreq);
		_left = CreateNode(pVFreq[pVFreq.size()-1]);
		_right = CreateNode(pVFreq[pVFreq.size()-2]);
		
		if(_vParent.empty()){
		}else{
			for(int i = 0; i < _vParent.size(); i++){
				if(_vParent[i]->info.Key == pVFreq[pVFreq.size()-1].Key){
					_left = _vParent[i];
					_vParent.erase(_vParent.begin()+i);
				}
				if(_vParent[i]->info.Key == pVFreq[pVFreq.size()-2].Key){
					_right = _vParent[i];
					_vParent.erase(_vParent.begin()+i);
				}
			}
		}
		nodeTree->left = _left;
		nodeTree->right = _right;
		
		_vParent.push_back(nodeTree);
		pVFreq.pop_back();
		pVFreq.pop_back();
		pVFreq.push_back(_sFreq);
		pVFreq = SortFrequency(pVFreq);
		
	}while(pVFreq.size() > 1);
	return _vParent[0];
}

void PaintTree(NodePtr pRoot, int pK){
	int i = pK;
	while((i--) > 0)
	{
		cout<<"\t";
	}
	cout<<pRoot->info.Key<<"("<<pRoot->info.Value<<")\n";
	if(pRoot->left != NULL){
		PaintTree(pRoot->left, ++pK);
	}
	if(pRoot->right != NULL){
		PaintTree(pRoot->right, pK);
	}
}

void Encryption(NodePtr pRoot, string pStrCode){
	if(pRoot->left == NULL && pRoot->right == NULL){
		sCode c;
		c.Key = pRoot->info.Key;
		c.Code = pStrCode;
		vCode.push_back(c);
		pStrCode = "";
	}else{
		string str = pStrCode;
		if(pRoot->left != NULL){
			str += "0";
			Encryption(pRoot->left, str);
		}
		if(pRoot->right != NULL){
			pStrCode += "1";
			Encryption(pRoot->right, pStrCode);
		}
	}
}

string ReturnCode(int pStr){
	string _strCode;
	sCode _c;
	for(int i = 0; i < vCode.size(); i++){
		_c = vCode[i];
		if(StringToChar(_c.Key) == pStr){
			_strCode = _c.Code;
		}
	}
	return _strCode;
}

void WriteFile(string url){
	char arrChar[Count];
    strInput.copy(arrChar, Count);
	
	ofstream fileOut(url.c_str());
	for(int i = 0; i < Count; i++){
		int _char = (int)arrChar[i];
		string _str = ReturnCode(_char);
		fileOut<<_str;
	}
	fileOut.close();
}

void Decryption(NodePtr pRoot, string url, string url2){
	string strCode[10000], str, strOut = "";
	ifstream file(url2.c_str());
	int CountOut = 0, _count = 0;
    if (file.is_open()){
    	while (!file.eof()){
            getline(file, str);
            strCode[_count] = str;
			_count++;
        }
    }
    file.close();
	// convert string[] to string
    for (int i = 0; strCode[i]!="\0"; i++){
	 	strOut = strOut + strCode[i];
	}
	CountOut = strOut.length();
	
	//convert string to char[]
    char arrCharOutput[CountOut];
    strOut.copy(arrCharOutput, CountOut);
	
	ofstream fileOut(url.c_str());
	NodePtr _node;
	Init(&_node);
	_node = pRoot;
	for (int i = 0; i < CountOut; i++){
	 	if(arrCharOutput[i] == '0'){
			if(_node->left != NULL){
				_node = _node->left;
			}
			if(_node->left == NULL && _node->right == NULL){
				fileOut<<_node->info.Key;
				_node = pRoot;
			}
		}
		if(arrCharOutput[i] == '1'){
			if(_node->right != NULL){
				_node = _node->right;
			}
			if(_node->left == NULL && _node->right == NULL){
				fileOut<<_node->info.Key;
				_node = pRoot;
			}
		}
	}
	fileOut.close();
}

int main(){
	vector<sFreq> vFrequency;
	NodePtr pTree;
	bool check = false;
	int work;
	do{
		cout<<"\n\n\n***** MENU *****";
		cout<<"\n1. Ma hoa.";
		cout<<"\n2. Giai ma.";
		cout<<"\n3. In ra bang tan so.";
		cout<<"\n4. In ra cay Huffman.";
		cout<<"\n5. In ra bang ma Huffman.";
		cout<<"\n0. Thoat.";
		cout<<"\nLua chon cong viec: "; cin>>work;
		switch(work){
			case 1: // read file and encryption
				{
					if(vFrequency.size() != 0){
						vFrequency.clear();
						vCode.clear();
						pTree = NULL;
					}
					system("cls");
					cout<<"Enter input file source: ";
					string url = "";
					cin>>url;
					vFrequency = ReadFile(url);				// read file
					vFrequency = SortFrequency(vFrequency);	// sort by frequency
					Init(&pTree);
					pTree = BuildTree(vFrequency);
					Encryption(pTree, "");			// create table code
					cout<<"\nEnter output file source:";
					string url3 = "";
					cin>>url3;
					WriteFile(url3);
					cout<<"Cryption code successfull!\n";
					check = true;
				}
				break;
			case 2: // decryption
				{
					if(check == true){
						cout<<"\nEnter input file source:";
						string url4 = "";
						cin>>url4;
						cout<<"\nEnter output file source:";
						string url2 = "";
						cin>>url2;
						Decryption(pTree, url2, url4);
						cout<<"Decryption successfull!";
					}else{
						cout<<"Do not decryption!";
					}
				}
				break;
			case 3: // show frequency table
				{
					if(check == true){
						for(int i = 0; i < vFrequency.size(); i++){
							cout<<vFrequency[i].Key<<" -> "<<vFrequency[i].Value<<endl;
						}
					}else{
						cout<<"Do not decryption!";
					}
				}
				break;
			case 4: // show Huffman struct
				{
					if(check == true){
						PaintTree(pTree, 0);
					}else{
						cout<<"Do not decryption!";
					}
				}
				break;
			case 5: // show Huffman table
				{
					if(check == true){
						sCode _sCode;
						for(int i = 0; i < vCode.size(); i++){
							_sCode = vCode[i];
							cout<<_sCode.Key<<" -> "<<_sCode.Code<<endl;
						}
					}else{
						cout<<"Do not decryption!";
					}
				}
				break;
			case 0:
				break;
			default:
				cout<<"Please choose a job!";
				break;
		}
	}while(work != 0);
	getch();
}
