#include "maze_pathfinding.h"

#define INF_MAX 1000
#define UNVISITED 0
#define VISITED 1
#define ARRIVED 0
#define TURNLEFT 14
#define TURNRIGHT 15
#define TURNBACK 16
#define MOVEFORE 11
enum DIRECTION{//maze��ľ��Է���
	UP = 1, LEFT, DOWN, RIGHT
};

struct mazenode{
	char wall;	//0-3λ�ֱ����ϣ����£��Ҳ������ϰ�����λ=0
	
	//·���滮��ʹ�õı���
	struct mazenode* parent;
	int priority;
	char visited;
};

//�����Թ���ά���飨���߽����һ�㣩
struct mazenode maze[8][8];

//���ǽ�ڣ�����λ�ã���0/��1/��2/��3��������λ�ò���Ҫ�ظ����
void AddWall (char x, char y, char side){
	switch(side){
	case UP:
		maze[y][x].wall |= 1;
		maze[y + 1][x].wall |= 4;
		break;
	case LEFT:
		maze[y][x].wall |= 2;
		maze[y][x - 1].wall |= 8;
		break;
	case DOWN:
		maze[y][x].wall |= 4;
		maze[y - 1][x].wall |= 1;
		break;
	case RIGHT:
		maze[y][x].wall |= 8;
		maze[y][x + 1].wall |= 2;
		break;
	default: break;
	}
}

//��ǽ����Ϣȫ�����
void ClearWalls (void){
	for (int i = 0; i < 8; i++){
		for (int j = 0; j < 8; j++){
			maze[i][j].wall = 0;
			//����߽�Χǽ��Ϣ
			if (j == 1) maze[i][j].wall |= 2;
			if (j == 6) maze[i][j].wall |= 8;
			if (i == 1) maze[i][j].wall |= 4;
			if (i == 6) maze[i][j].wall |= 1;
		}
	}
	//��ӳ��طֽ絲����Ϣ
	for (int i = 1; i < 6; i++){
		AddWall(i, 6 - i, UP);
		AddWall(i, 6 - i, RIGHT);
	}
}

//��������ǰ����xy���꣬Ŀ��λ��xy���꣨��Χ1-6,Ҫ��ԭ��������ϼ�1������ǰλ���ϰ������0-2λ�ֱ����ң�ǰ����������ϰ���
unsigned char PathFinding (char currentDir, char posx, char posy, char aimx, char aimy, char wallrefresh){
	unsigned char decision = 255;

//����յ��غϷ���ARRIVED = 0
	if (posx == aimx && posy == aimy) return ARRIVED;

//����ǽ����Ϣ
	switch(currentDir){
	case RIGHT:
		if (wallrefresh & 4) AddWall(posx,posy,UP);
		if (wallrefresh & 2) AddWall(posx,posy,RIGHT);
		if (wallrefresh & 1) AddWall(posx,posy,DOWN);
		break;
	case UP:
		if (wallrefresh & 4) AddWall(posx,posy,LEFT);
		if (wallrefresh & 2) AddWall(posx,posy,UP);
		if (wallrefresh & 1) AddWall(posx,posy,RIGHT);
		break;
	case DOWN:
		if (wallrefresh & 4) AddWall(posx,posy,RIGHT);
		if (wallrefresh & 2) AddWall(posx,posy,DOWN);
		if (wallrefresh & 1) AddWall(posx,posy,LEFT);
		break;
	case LEFT:
		if (wallrefresh & 4) AddWall(posx,posy,DOWN);
		if (wallrefresh & 2) AddWall(posx,posy,LEFT);
		if (wallrefresh & 1) AddWall(posx,posy,UP);
		break;
	default: break;
	}

//��չ滮·�ߵ��м���Ϣ
	for (int i = 0; i < 8; i++){
		for (int j = 0; j < 8; j++){
		    maze[i][j].parent = 0;
		    maze[i][j].priority = INF_MAX;	//��ʼ���ȼ��ø�
			maze[i][j].visited = UNVISITED;
		}
	}

//Dijkstra�滮·��
	maze[posy][posx].priority = 0;
	struct mazenode* selectednode = 0;
	struct mazenode* old_snode = 0;
	while (1){
		//ѡ��δ���ʽڵ��е�������ȼ�����
		int shortest = INF_MAX;
		for (int i = 1; i < 7; i++){
			for (int j = 1; j < 7; j++){
				if (maze[i][j].visited == UNVISITED && maze[i][j].priority < shortest){
					shortest = maze[i][j].priority;
					selectednode = &maze[i][j];
				}
			}
		}
		if (old_snode == selectednode) return 255;	//��ѭ��return
		selectednode -> visited = VISITED;
		//��Ŀ��㱻ѡ��ʱѭ����ֹ
		if (maze[aimy][aimx].visited == VISITED) break;
		//�Դ˽ڵ���ھ����ȼ�������
		struct mazenode* rightnode = (selectednode + 1);
		struct mazenode* leftnode  = (selectednode - 1);
		struct mazenode* downnode  = (selectednode - 8);
		struct mazenode* upnode    = (selectednode + 8);
		//dirjudge
		int selectindex = selectednode - &maze[0][0];
		int parentindex = (selectednode -> parent) ? selectednode -> parent - &maze[0][0] : selectindex;
		int dirjudge = selectindex - parentindex;
		int pathweight = INF_MAX;
		//right
		if ((selectednode -> wall & 8) != 0) pathweight = INF_MAX;
		else if (dirjudge == 0 || dirjudge == 1) pathweight = 1;
		else if (dirjudge == -1) pathweight = 3;
		else  pathweight = 2;
		if (selectednode -> priority + pathweight < rightnode -> priority){
			rightnode -> priority = selectednode -> priority + pathweight;
			rightnode -> parent = selectednode;
		}
		//left
		if ((selectednode -> wall & 2) != 0) pathweight = INF_MAX;
		else if (dirjudge == 0 || dirjudge == -1) pathweight = 1;
		else if (dirjudge == 1) pathweight = 3;
		else  pathweight = 2;
		if (selectednode -> priority + pathweight < leftnode -> priority){
			leftnode -> priority = selectednode -> priority + pathweight;
			leftnode -> parent = selectednode;
		}
		//down
		if ((selectednode -> wall & 4) != 0) pathweight = INF_MAX;
		else if (dirjudge == 0 || dirjudge == -8) pathweight = 1;
		else if (dirjudge == 8) pathweight = 3;
		else  pathweight = 2;
		if (selectednode -> priority + pathweight < downnode -> priority){
			downnode -> priority = selectednode -> priority + pathweight;
			downnode -> parent = selectednode;
		}
		//up
		if ((selectednode -> wall & 1) != 0) pathweight = INF_MAX;
		else if (dirjudge == 0 || dirjudge == 8) pathweight = 1;
		else if (dirjudge == -8) pathweight = 3;
		else  pathweight = 2;
		if (selectednode -> priority + pathweight < upnode -> priority){
			upnode -> priority = selectednode -> priority + pathweight;
			upnode -> parent = selectednode;
		}
		//�ж���ѭ��
		old_snode = selectednode;
	}

//���߽��
//���ص�һ��
	//struct mazenode* node = &maze[aimy][aimx];
	//struct mazenode* startnode = &maze[posy][posx];
	while (1){
		//ѡ��δ���ʽڵ��е�������ȼ�����
		int shortest = INF_MAX;
		for (int i = 1; i < 7; i++){
			for (int j = 1; j < 7; j++){
				if (maze[i][j].visited == UNVISITED && maze[i][j].priority < shortest){
					shortest = maze[i][j].priority;
					selectednode = &maze[i][j];
				}
			}
		}
		if (old_snode == selectednode) return 255;	//��ѭ��return
		selectednode -> visited = VISITED;
		//��Ŀ��㱻ѡ��ʱѭ����ֹ
		if (maze[aimy][aimx].visited == VISITED) break;
		//�Դ˽ڵ���ھ����ȼ�������
		struct mazenode* rightnode = (selectednode + 1);
		struct mazenode* leftnode  = (selectednode - 1);
		struct mazenode* downnode  = (selectednode - 8);
		struct mazenode* upnode    = (selectednode + 8);
		//dirjudge
		int selectindex = selectednode - &maze[0][0];
		int parentindex = (selectednode -> parent) ? selectednode -> parent - &maze[0][0] : selectindex;
		int dirjudge = selectindex - parentindex;
		int pathweight = INF_MAX;
		//right
		if ((selectednode -> wall & 8) != 0) pathweight = INF_MAX;
		else if (dirjudge == 0 || dirjudge == 1) pathweight = 1;
		else if (dirjudge == -1) pathweight = 3;
		else  pathweight = 2;
		if (selectednode -> priority + pathweight < rightnode -> priority){
			rightnode -> priority = selectednode -> priority + pathweight;
			rightnode -> parent = selectednode;
		}
		//left
		if ((selectednode -> wall & 2) != 0) pathweight = INF_MAX;
		else if (dirjudge == 0 || dirjudge == -1) pathweight = 1;
		else if (dirjudge == 1) pathweight = 3;
		else  pathweight = 2;
		if (selectednode -> priority + pathweight < leftnode -> priority){
			leftnode -> priority = selectednode -> priority + pathweight;
			leftnode -> parent = selectednode;
		}
		//down
		if ((selectednode -> wall & 4) != 0) pathweight = INF_MAX;
		else if (dirjudge == 0 || dirjudge == -8) pathweight = 1;
		else if (dirjudge == 8) pathweight = 3;
		else  pathweight = 2;
		if (selectednode -> priority + pathweight < downnode -> priority){
			downnode -> priority = selectednode -> priority + pathweight;
			downnode -> parent = selectednode;
		}
		//up
		if ((selectednode -> wall & 1) != 0) pathweight = INF_MAX;
		else if (dirjudge == 0 || dirjudge == 8) pathweight = 1;
		else if (dirjudge == -8) pathweight = 3;
		else  pathweight = 2;
		if (selectednode -> priority + pathweight < upnode -> priority){
			upnode -> priority = selectednode -> priority + pathweight;
			upnode -> parent = selectednode;
		}
		//startpoint
		if (dirjudge == 0){
			switch(currentDir){
			case UP:
				downnode -> priority += 2;
				rightnode -> priority += 1;
				leftnode -> priority += 1;
				break;
			case DOWN:
				upnode -> priority += 2;
				rightnode -> priority += 1;
				leftnode -> priority += 1;
				break;
			case LEFT:
				rightnode -> priority += 2;
				upnode -> priority += 1;
				downnode -> priority += 1;
				break;
			case RIGHT:
				leftnode -> priority += 2;
				upnode -> priority += 1;
				downnode -> priority += 1;
				break;
			default: break;
			}
		}
		//�ж���ѭ��
		old_snode = selectednode;
	}
	return decision;
}

/*********************************************************************************/
/*********************************************************************************/
/*********************************************************************************/
/*********************************************************************************/
/*********************************************************************************/

unsigned char PersonFinding (char currentDir, char posx, char posy, char p1bx, char p1by, char p2bx, char p2by, char wallrefresh){
	unsigned char decision = 255;

	return decision;
}
