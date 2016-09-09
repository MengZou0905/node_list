#include <stdio.h>
#include <stdlib.h>

typedef struct set
{
	int id;
	int eleNum;
	int freSep;
	int element[150];
	struct set *next;
}set;
set *setHead = NULL;
int ReadData(char *path)
{
	FILE *fr;
	char buf[300];
	int id = 0;

	set *pre = NULL;
	set *cur = NULL;
	int totalNum = 0;

	fr = fopen(path, "r");
	int time = 0;
	while (fgets(buf, sizeof(buf), fr) && time < 5){
		time ++;
		//printf("%s", buf);
		cur = (set *)malloc(sizeof(set));
		cur->next = NULL;
		cur->id = id;
		cur->eleNum = 0;
		if (setHead == NULL){
			setHead = cur;
			pre = cur;
		}
		else{
			pre->next = cur;
			pre = cur;
		}
		for (int i = 0; strchr(buf, ' '); i++){
			cur->eleNum = i + 1;
			sscanf(buf, "%d", &cur->element[i]);
			//printf("cur->element[%d]= %d\n", i, cur->element[i]);
			//printf("buf:%s\n", buf);
			char *firstBlank = strchr(buf, ' ');
			int blankPos = firstBlank - buf;
			int strLen = strlen(buf);
			char tempBuf[300];
			memset(tempBuf, '\0', 300);
			//printf("blankPos:%d\n", blankPos);
			//printf("strLen:%d\n", strLen);
			//printf("cpyStart:%d\n", buf + blankPos + 1);
			//printf("cpyLen:%d\n", strLen - blankPos);
			strncpy(tempBuf, buf + blankPos + 1, strLen - blankPos);
			tempBuf[strLen - blankPos] = '\0';
			memset(buf, '\0', strLen);
			strcpy(buf, tempBuf);
			//printf("buf:%s\n",buf);	
		}
		id++;
		totalNum += cur->eleNum;
		/*
		printf("id: %d: ",id-1);
		for (int i = 0; i < cur->eleNum; i++)
		printf("%d,", cur->element[i]);
		printf("\n");
		*/
	}
	fclose(fr);

	return totalNum;
}

int itemCount[3000] = { 0 };
void CountData(){
	for (set *setCur = setHead; setCur != NULL; setCur = setCur->next){
		for (int i = 0; i < setCur->eleNum; i++){
			itemCount[setCur->element[i]] += 1;
		}
	}
	/*
	for (int i = 0; i < 3000; i++)
		printf("%d: %d times\n", i, itemCount[i]);
	*/
}

void RearrangeData(int totalNum, float threshold){
	for (set *setCur = setHead; setCur != NULL; setCur = setCur->next){
		for (int i = 0; i < setCur->eleNum; i++){
			for (int j = i + 1; j < setCur->eleNum; j++){
				if (itemCount[setCur->element[i]] < itemCount[setCur->element[j]]){
					int temp = setCur->element[i];
					setCur->element[i] = setCur->element[j];
					setCur->element[j] = temp;
				}
			}
		}
	}
}

void CheckSet()
{
	for (set *setCur = setHead; setCur != NULL; setCur = setCur->next){
		printf("id:%d\n",setCur->id);
		printf("num:%d\n",setCur->eleNum);
		for (int i = 0; i < setCur->eleNum; i++){
			printf("%d(%d) ", setCur->element[i], itemCount[setCur->element[i]]);
		}
		printf("\n\n");
	}
}

typedef struct nodeRec
{
	int setId;
	struct nodeRec *next;
}nodeRec;
typedef struct node
{
	int nodeId;
	int DLR;
	int LRD;
	struct nodeRec *firstRec;
	struct nodeRec *lastRec;
	struct node *son;
	struct node *bro;
	struct node *par;
	leaf *sonleaf;
}node;
node *root = NULL;
typedef struct leafRec
{
	int leafId;
	nodeRec *firstRec;
	nodeRec *lastRec;
	struct leafRec *next;
}leafRec;
typedef struct leaf
{
	int DLR;
	int LRD;
	int leafRecNum;
	leafRec *firstNotFre;
	node *par;
	node *bro;
}leaf;
void BuildTree()
{
	node *nodeCur = NULL;
	node *nodePre = NULL;
	node *nodeLeftBro = NULL;
	leaf *leafCur = NULL;
	nodeRec *newRec = NULL;
	leafRec * recCur = NULL; 
	leafRec * recPre = NULL;

	root = (node *)malloc(sizeof(node));
	root->bro = NULL;
	root->firstRec = NULL;
	root->lastRec = NULL;
	root->nodeId = -1;
	root->par = NULL;
	root->son = NULL;
	root->sonleaf = NULL;

	for (set *setCur = setHead; setCur != NULL; setCur = setCur->next){
		nodePre = root;
		nodeCur = NULL;
		leafCur = NULL;
		for (int i = 0; i < setCur->freSep; i++){
			if (nodeCur == NULL){
				nodeCur = (node *)malloc(sizeof(node));
				nodeCur->bro = NULL;
				nodeCur->firstRec = (nodeRec *)malloc(sizeof(nodeRec));
				nodeCur->firstRec->setId = setCur->id;
				nodeCur->firstRec->next = NULL;
				nodeCur->lastRec = nodeCur->firstRec;
				nodeCur->nodeId = setCur->element[i];
				nodeCur->son = NULL;
				nodeCur->sonleaf = NULL;
				nodeCur->par = nodePre;
				nodePre->son = nodeCur;
				nodePre = nodeCur;
				
			}
			else{
				for (; nodeCur != NULL && nodeCur->nodeId != setCur->element[i]; nodeLeftBro = nodeCur, nodeCur = nodeCur->bro);
				if (nodeCur == NULL){
					nodeCur = (node *)malloc(sizeof(node));
					nodeCur->bro = NULL;
					nodeCur->firstRec = (nodeRec *)malloc(sizeof(nodeRec));
					nodeCur->firstRec->setId = setCur->id;
					nodeCur->firstRec->next = NULL;
					nodeCur->lastRec = nodeCur->firstRec;
					nodeCur->nodeId = setCur->element[i];
					nodeCur->par = nodePre;
					nodeCur->son = NULL;
					nodeCur->sonleaf = NULL;
					nodeLeftBro->bro = nodeCur;
				}
				else{
					newRec = (nodeRec *)malloc(sizeof(nodeRec));
					newRec->setId = setCur->id;
					newRec->next = NULL;
					nodeCur->lastRec->next = newRec;
					nodeCur->lastRec = newRec;
				}
				nodePre = nodeCur;
			}
			if (i < setCur->freSep - 1)
				nodeCur = nodePre->son;
		}
		for (int i = setCur->freSep; i < setCur->eleNum; i++){
			leafCur = nodePre->sonleaf;
			if (leafCur == NULL){
				leafCur = (leaf *)malloc(sizeof(leaf));
				leafCur->bro = nodePre->son;
				leafCur->firstNotFre = (leafRec *)malloc(sizeof(leafRec));
				leafCur->firstNotFre->firstRec = (nodeRec *)malloc(sizeof(nodeRec));
				leafCur->firstNotFre->firstRec->setId = setCur->id;
				leafCur->firstNotFre->firstRec->next = NULL;
				leafCur->firstNotFre->lastRec = leafCur->firstNotFre->firstRec;
				leafCur->firstNotFre->leafId = setCur->element[i];
				leafCur->firstNotFre->next = NULL;
				leafCur->leafRecNum = 1;
				leafCur->par = nodePre;
				nodeCur->sonleaf = leafCur;
			}
			else{
				recCur = leafCur->firstNotFre;
				recPre = NULL;
				for (; recCur != NULL && recCur->leafId != setCur->element[i]; recPre = recCur, recCur = recCur->next);
				if (recCur == NULL){
					recCur = (leafRec *)malloc(sizeof(leafRec));
					recCur->firstRec = (nodeRec *)malloc(sizeof(nodeRec));
					recCur->firstRec->setId = setCur->id;
					recCur->firstRec->next = NULL;
					recCur->lastRec = recCur->firstRec;
					recCur->leafId = setCur->element[i];
					recCur->next = NULL;
					recPre->next = recCur;
					
				}
				else{
					newRec = (nodeRec *)malloc(sizeof(nodeRec));
					newRec->setId = setCur->id;
					newRec->next = NULL;
					recCur->lastRec->next = newRec;
					recCur->lastRec = newRec;
				}
			}
		}
	}
}

int DLR(int index, node *nodePre, node *nodeCur, leaf *leafCur)
{
	if (nodeCur == NULL && leafCur != NULL){
		leafCur->DLR == index;
		leafRec * temp = leafCur;
		for (int i = 0; i < leafCur->leafRecNum; i++){
			printf("%d ", temp->leafId);
			temp = temp->next;
		}
		printf("(DLR):%d\n", leafCur->DLR);
		DLR(index + 1, nodePre, leafCur->bro, NULL);
	}
	else{
		if (nodeCur != NULL && leafCur == NULL){
			nodeCur->DLR = index;
			printf("%d (DLR):%d\n",nodeCur->nodeId, nodeCur->DLR);
			if (nodeCur->sonleaf != NULL)
				DLR(index + 1, nodeCur, NULL, nodeCur->sonleaf);
			else
				DLR(index + 1, nodeCur, nodeCur->son, NULL);
		}
		else{
			if (nodeCur == NULL && leafCur == NULL){
				if (nodePre == root)
					return 0;
				else{
					DLR(index, nodePre->par, nodePre->bro, NULL);
				}
			}
		}
	} 
}

int LRD(int index, node *nodePre, node *nodeCur, leaf *leafCur)
{
	int temp = 0;
	if (leafCur != NULL && nodeCur == NULL){
		leafCur->LRD = index;
		leafRec * temp = leafCur;
		for (int i = 0; i < leafCur->leafRecNum; i++){
			printf("%d ", temp->leafId);
			temp = temp->next;
		}
		printf("(DLR):%d\n", leafCur->LRD);
		LRD(index + 1, nodePre, leafCur->bro, NULL);
	}
	else{
		if (leafCur == NULL && nodeCur != NULL){
			if (nodeCur->sonleaf != NULL)
				LRD(index, nodeCur, NULL, nodeCur->sonleaf);
			else
				LRD(index, nodeCur, nodeCur->son, NULL);
		}
		else{
			if (leafCur == NULL && nodeCur == NULL){
				nodePre->LRD == index;
				printf("%d (DLR):%d\n", nodePre->nodeId, nodePre->LRD);
				if (nodePre == root)
					return 0;
				else
					LRD(index + 1, nodePre->par, nodePre->bro, NULL);
			}
		}
	}
}

void CheckTree()
{

}

void GenNodeList()
{

}

void Query()
{

}
int main()
{
	char *filePath = ".//data//mushroom.dat";
	int totalNum = 0;
	float threshold = 0.25;

	totalNum = ReadData(filePath);
	CountData();
	CheckSet();
	RearrangeData(totalNum,threshold);
	CheckSet();
	BuildTree();
	CheckTree();
	DLR(0, NULL, root, NULL);
	LRD(0, NULL, root, NULL);
	//CheckTree();
	//GenNodeList();
	//Query();
	
	system("pause");
	return 0;
}
