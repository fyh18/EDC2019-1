#ifndef _MAZE_PATHFINDING_H_
#define _MAZE_PATHFINDING_H_

/*״̬�����壨Ӧ��main����ͬ��
#define TURNLEFT 14
#define TURNRIGHT 15
#define TURNBACK 16
#define MOVEFORE 11

#define DIRECTLEFT 17
#define DIRECTRIGHT 18
#define DIRECTBACK 19
*/

//����ϰ����ڵ�����λ��:��Χ1-6,Ҫ��ԭ��������ϼ�1��ѡ�ߣ���0/��1/��2/��3�������ڽڵ���Χǽ����Ҫ�ظ����
extern void AddWall (char x, char y, char side);

//��ǽ����Ϣȫ����գ������Χǽ���м�ֽ��ߣ�
extern void ClearWalls (void);

/*
·���滮����
��������ǰ���򣨿���ֱ�Ӹ�UP��LEFT��DOWN��RIGHT����main�ж�����ͬ����
      xy���꣬Ŀ��λ��xy���꣨��Χ1-6,Ҫ��ԭ��������ϼ�1����
      ��ǰλ���ϰ������0-3λ�ֱ����ң�ǰ���󣬺�������ϰ���
����ֵ�����ؾ���״̬����ARRIVED = 0, fail = 255
*/
extern unsigned char PathFinding (char currentDir, char posx, char posy, char aimx, char aimy, char wallrefresh);
extern unsigned char PersonFinding (char currentDir, char posx, char posy, char p1bx, char p1by, char p2bx, char p2by, char wallrefresh);

#endif
