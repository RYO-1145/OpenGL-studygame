/*レポート３　T14CS045  細谷 燎平*/
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>
#include <errno.h>
#include <ctype.h>
#define KEY_ESC 27
#define KEY_SPACE 32
#define KEY_S 115
#define KEY_W 119
#define PI2 2.0*3.1415926534

void polarview( void );
void resetview( void );
void mySolidCylinder( float r, float h, int n );
int Signal=1;
int sg=0,op=0;
int xBegin = 5, yBegin = 5;
int mButton;
int num;
int icon;
int q=1;
float distance, twist, elevation, azimuth,xside=0,yside=0;
float nearw = 2.0, farw = 10000.0, fovy = 60.0;
//視点に関する変数
float lookx,looky,lookz,pointx,pointy,pointz;
//以下はアニメーションのための変数※各説明は時間がかかるため省く
float trans=0.0f;
GLfloat angle = 0.0f,door = PI2/2,close=0.0f,walk=0.0f,arm=0.0f;
GLfloat jumpx=0.0f,jumpy=0.0f,ball=0.0f,lost=0.0f,kk=0.0f,plus=0.0f,ss=0.0f;
GLfloat a=1.0f,so=1.0f;
int sc=17,nn=0,zxc=0,sic=0;

/*  --- テクスチャファイルを読み込むための設定 ---  */
#define imageWidth 240
#define imageHeight 240
#define NumOfTextureFiles 58
/*それぞれのテクスチャを読み込む*/
static const char *images[] = { 
  "./tex/t14cs045_alb.ppm","./tex/t14cs045_bodyfront.ppm","./tex/t14cs045_bodyback.ppm","./tex/t14cs045_updown.ppm",
  "./tex/t14cs045_headup.ppm","./tex/t14cs045_headdown.ppm","./tex/t14cs045_headleft.ppm","./tex/t14cs045_headright.ppm","./tex/t14cs045_face.ppm","./tex/t14cs045_faceback.ppm",
  "./tex/t14cs045_nihon.ppm",
  "./tex/t14cs045_easy.ppm","./tex/t14cs045_normal.ppm","./tex/t14cs045_hard.ppm","./tex/t14cs045_ncnc.ppm",
  "./tex/t14cs045_red.ppm","./tex/t14cs045_blue.ppm",
  "./tex/t14cs045_num0.ppm","./tex/t14cs045_num1.ppm","./tex/t14cs045_num2.ppm","./tex/t14cs045_num3.ppm","./tex/t14cs045_num4.ppm","./tex/t14cs045_num5.ppm","./tex/t14cs045_num6.ppm","./tex/t14cs045_num7.ppm","./tex/t14cs045_num8.ppm",
  "./tex/t14cs045_Easy.ppm","./tex/t14cs045_Normal.ppm","./tex/t14cs045_Hard.ppm","./tex/t14cs045_Ncnc.ppm",
  "./tex/t14cs045_q1.ppm","./tex/t14cs045_q2.ppm","./tex/t14cs045_q3.ppm","./tex/t14cs045_q4.ppm","./tex/t14cs045_q5.ppm","./tex/t14cs045_q6.ppm","./tex/t14cs045_q7.ppm","./tex/t14cs045_q8.ppm",
  "./tex/t14cs045_n1.ppm","./tex/t14cs045_n2.ppm","./tex/t14cs045_n3.ppm","./tex/t14cs045_n4.ppm","./tex/t14cs045_n5.ppm","./tex/t14cs045_n6.ppm","./tex/t14cs045_n7.ppm","./tex/t14cs045_n8.ppm",
  "./tex/t14cs045_op1.ppm","./tex/t14cs045_op2.ppm","./tex/t14cs045_op3.ppm","./tex/t14cs045_op4.ppm","./tex/t14cs045_op5.ppm","./tex/t14cs045_nico1.ppm","./tex/t14cs045_nico2.ppm","./tex/t14cs045_nico3.ppm","./tex/t14cs045_nico4.ppm","./tex/t14cs045_kk.ppm","./tex/t14cs045_com1.ppm","./tex/t14cs045_com2.ppm",
};

GLubyte image[imageHeight][imageWidth][3];
GLint texID[NumOfTextureFiles];

char* get_word(void);
void LoadASCIIPPMImage(const char* filename);

static FILE* texturefile;
static char readbuf[256];
static char word[256];
static char* ptr = readbuf;
static char* ptr2 = word;
/*  ---------------------------------------  */
void InitLight()
{
  //起動してすぐに行われる視点変更とともに光源の方向も変更する
  if(angle>10){
    if(angle<20){
      if(so>-0.1)
	so=so-0.1;
    }
  }
	GLfloat ambient[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat diffuse[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat specular[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat position[4] = { 0.7f, 1.0f, so, 0.0f };

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0, GL_POSITION, position);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
}


//どんなCGを表示するか設定する関数
void DrawHuman(void)
{
  //テクスチャを使うCGは全てここで行う
  //テクスチャを貼っているので材質は関係ないので初期設定のまま
  float diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
  float specular[] = { 1.0, 1.0, 1.0, 1.0 };
  float ambient[] = { 1.0, 1.0, 1.0, 1.0 };
  float ambient_light[] = { 2.5, 2.5, 2.5, 1.0 };
  float blueEmission[] = {1.0, 1.0, 1.0, 1.0}; 
  float light[] = { 1.0, 1.0, 3.0, 1.0 };
  
  glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  glPushMatrix();
  polarview();
  glEnable( GL_DEPTH_TEST );
  
  glLightfv( GL_LIGHT0, GL_DIFFUSE,  diffuse);
  glLightfv( GL_LIGHT0, GL_SPECULAR, specular);
  glLightfv( GL_LIGHT0, GL_AMBIENT, ambient_light);
  
  glMaterialfv( GL_FRONT, GL_DIFFUSE, diffuse );
  glMaterialfv( GL_FRONT, GL_SPECULAR, specular );
  glMaterialfv( GL_FRONT, GL_AMBIENT, ambient );
  glMaterialf( GL_FRONT, GL_SHININESS, 51.2 );
  
  glEnable( GL_LIGHTING );
  glEnable(GL_NORMALIZE);

  glPushMatrix();
  glTranslatef(jumpx,jumpy,walk);
  
  
  //頭部
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texID[4]);
  glBegin(GL_QUADS);
  glTexCoord2f(0.0,0.33); glVertex3f(-0.4,1.4,-0.4);
  glTexCoord2f(0.0,1.0); glVertex3f(-0.4,1.4,0.4);
  glTexCoord2f(0.66,1.0); glVertex3f(0.4,1.4,0.4);
  glTexCoord2f(0.66,0.33); glVertex3f(0.4,1.4,-0.4);
  glEnd();
  glDisable(GL_TEXTURE_2D);

  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texID[5]);
  glBegin(GL_QUADS);
  glTexCoord2f(0.0,0.33); glVertex3f(-0.4,0.602,-0.4);
  glTexCoord2f(0.0,1.0); glVertex3f(-0.4,0.602,0.4);
  glTexCoord2f(0.66,1.0); glVertex3f(0.4,0.602,0.4);
  glTexCoord2f(0.66,0.33); glVertex3f(0.4,0.602,-0.4);
  glEnd();
  glDisable(GL_TEXTURE_2D);

  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texID[6]);
  glBegin(GL_QUADS);
  glTexCoord2f(0.66,1.0); glVertex3f(0.4,0.6,-0.4);
  glTexCoord2f(0.66,0.33); glVertex3f(0.4,1.4,-0.4);
  glTexCoord2f(0.0,0.33); glVertex3f(0.4,1.4,0.4);
  glTexCoord2f(0.0,1.0); glVertex3f(0.4,0.6,0.4);
  glEnd();
  glDisable(GL_TEXTURE_2D);

  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texID[7]);
  glBegin(GL_QUADS);
  glTexCoord2f(0.66,1.0); glVertex3f(-0.4,0.6,-0.4);
  glTexCoord2f(0.66,0.33); glVertex3f(-0.4,1.4,-0.4);
  glTexCoord2f(0.0,0.33); glVertex3f(-0.4,1.4,0.4);
  glTexCoord2f(0.0,1.0); glVertex3f(-0.4,0.6,0.4);
  glEnd();
  glDisable(GL_TEXTURE_2D);

  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texID[8]);
  glBegin(GL_QUADS);
  glTexCoord2f(0.66,1.0); glVertex3f(-0.4,0.6,0.4);
  glTexCoord2f(0.66,0.33); glVertex3f(-0.4,1.4,0.4);
  glTexCoord2f(0.0,0.33); glVertex3f(0.4,1.4,0.4);
  glTexCoord2f(0.0,1.0); glVertex3f(0.4,0.6,0.4);
  glEnd();
  glDisable(GL_TEXTURE_2D);

  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texID[9]);
  glBegin(GL_QUADS);
  glTexCoord2f(0.66,1.0); glVertex3f(-0.4,0.6,-0.4);
  glTexCoord2f(0.66,0.33); glVertex3f(-0.4,1.4,-0.4);
  glTexCoord2f(0.0,0.33); glVertex3f(0.4,1.4,-0.4);
  glTexCoord2f(0.0,1.0); glVertex3f(0.4,0.6,-0.4);
  glEnd();
  glDisable(GL_TEXTURE_2D);
  
  //胴体
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texID[1]);
  glBegin(GL_QUADS);
  glTexCoord2f(0.0,0.0); glVertex3f(-0.4,-0.6,0.2);
  glTexCoord2f(0.0,1.0); glVertex3f(-0.4,0.6,0.2);
  glTexCoord2f(0.67,1.0); glVertex3f(0.4,0.6,0.2);
  glTexCoord2f(0.67,0.0); glVertex3f(0.4,-0.6,0.2);
  glEnd();
  glDisable(GL_TEXTURE_2D);

  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texID[2]);
  glBegin(GL_QUADS);
  glTexCoord2f(0.0,0.0); glVertex3f(-0.4,-0.6,-0.2);
  glTexCoord2f(0.0,1.0); glVertex3f(-0.4,0.6,-0.2);
  glTexCoord2f(0.67,1.0); glVertex3f(0.4,0.6,-0.2);
  glTexCoord2f(0.67,0.0); glVertex3f(0.4,-0.6,-0.2);
  glEnd();
  glDisable(GL_TEXTURE_2D);

  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texID[0]);
  glBegin(GL_QUADS);
  glTexCoord2f(0.66,0.0); glVertex3f(0.4,-0.6,-0.2);
  glTexCoord2f(0.66,1.0); glVertex3f(0.4,0.6,-0.2);
  glTexCoord2f(1.0,1.0); glVertex3f(0.4,0.6,0.2);
  glTexCoord2f(1.0,0.0); glVertex3f(0.4,-0.6,0.2);
  glEnd();
  glDisable(GL_TEXTURE_2D);

  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texID[0]);
  glBegin(GL_QUADS);
  glTexCoord2f(0.66,0.0); glVertex3f(-0.4,-0.6,-0.2);
  glTexCoord2f(1.0,0.0); glVertex3f(-0.4,-0.6,0.2);
  glTexCoord2f(1.0,1.0); glVertex3f(-0.4,0.6,0.2);
  glTexCoord2f(0.66,1.0); glVertex3f(-0.4,0.6,-0.2);
  glEnd();
  glDisable(GL_TEXTURE_2D);
 
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texID[3]);
  glBegin(GL_QUADS);
  glTexCoord2f(0.67,1.0); glVertex3f(-0.4,-0.6,-0.2);
  glTexCoord2f(0.67,0.67); glVertex3f(-0.4,-0.6,0.2);
  glTexCoord2f(0.0,0.67); glVertex3f(0.4,-0.6,0.2);
  glTexCoord2f(0.0,1.0); glVertex3f(0.4,-0.6,-0.2);
  glEnd();
  glDisable(GL_TEXTURE_2D);

  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texID[3]);
  glBegin(GL_QUADS);
  glTexCoord2f(0.0,0.33); glVertex3f(-0.4,0.6,0.2);
  glTexCoord2f(0.0,0.66); glVertex3f(-0.4,0.6,-0.2);
  glTexCoord2f(0.66,0.66); glVertex3f(0.4,0.6,-0.2);
  glTexCoord2f(0.66,0.33); glVertex3f(0.4,0.6,0.2);
  glEnd();
  glDisable(GL_TEXTURE_2D);

 
  //左足
  glPushMatrix();
  if(angle<90){
  glTranslatef(0.2,-0.8,0.0);
  glRotatef(45*(-1)*sin(walk),1.0,0.0,0.0);
  glTranslatef(-0.2,0.8,0.0);
  }
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texID[3]);
  glBegin(GL_QUADS);
  glTexCoord2f(0.66,0.33); glVertex3f(0.0,-0.603,-0.2);
  glTexCoord2f(0.66,0.66); glVertex3f(0.0,-0.603,0.2);
  glTexCoord2f(1.0,0.66); glVertex3f(0.4,-0.603,0.2);
  glTexCoord2f(1.0,0.33); glVertex3f(0.4,-0.603,-0.2);
  glEnd();
  glDisable(GL_TEXTURE_2D);

  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texID[3]);
  glBegin(GL_QUADS);
  glTexCoord2f(0.66,0.66); glVertex3f(0.0,-1.8,-0.2);
  glTexCoord2f(0.66,1.0); glVertex3f(0.0,-1.8,0.2);
  glTexCoord2f(1.0,1.0); glVertex3f(0.4,-1.8,0.2);
  glTexCoord2f(1.0,0.66); glVertex3f(0.4,-1.8,-0.2);
  glEnd();
  glDisable(GL_TEXTURE_2D);

  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texID[0]);
  glBegin(GL_QUADS);
  glTexCoord2f(0.33,0.0); glVertex3f(0.4,-1.8,-0.2);
  glTexCoord2f(0.66,0.0); glVertex3f(0.4,-1.8,0.2);
  glTexCoord2f(0.66,1.0); glVertex3f(0.4,-0.6,0.2);
  glTexCoord2f(0.33,1.0); glVertex3f(0.4,-0.6,-0.2);
  glEnd();
  glDisable(GL_TEXTURE_2D);

  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texID[0]);
  glBegin(GL_QUADS);
  glTexCoord2f(0.33,0.0); glVertex3f(0.0,-1.8,-0.2);
  glTexCoord2f(0.66,0.0); glVertex3f(0.0,-1.8,0.2);
  glTexCoord2f(0.66,1.0); glVertex3f(0.0,-0.6,0.2);
  glTexCoord2f(0.33,1.0); glVertex3f(0.0,-0.6,-0.2);
  glEnd();
  glDisable(GL_TEXTURE_2D);

  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texID[0]);
  glBegin(GL_QUADS);
  glTexCoord2f(0.33,0.0); glVertex3f(0.0,-1.8,0.2);
  glTexCoord2f(0.66,0.0); glVertex3f(0.4,-1.8,0.2);
  glTexCoord2f(0.66,1.0); glVertex3f(0.4,-0.6,0.2);
  glTexCoord2f(0.33,1.0); glVertex3f(0.0,-0.6,0.2);
  glEnd();
  glDisable(GL_TEXTURE_2D);

  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texID[0]);
  glBegin(GL_QUADS);
  glTexCoord2f(0.33,0.0); glVertex3f(0.0,-1.8,-0.2);
  glTexCoord2f(0.66,0.0); glVertex3f(0.4,-1.8,-0.2);
  glTexCoord2f(0.66,1.0); glVertex3f(0.4,-0.6,-0.2);
  glTexCoord2f(0.33,1.0); glVertex3f(0.0,-0.6,-0.2);
  glEnd();
  glDisable(GL_TEXTURE_2D);
  glPopMatrix();

  
  //右足
  glPushMatrix();
  if(angle<90){
  glTranslatef(-0.2,-0.8,0.0);
  glRotatef(45*sin(walk),1.0,0.0,0.0);
  glTranslatef(0.2,0.8,0.0);
  }
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texID[3]);
  glBegin(GL_QUADS);
  glTexCoord2f(1.0,0.33); glVertex3f(-0.4,-0.603,-0.2);
  glTexCoord2f(1.0,0.66); glVertex3f(-0.4,-0.603,0.2);
  glTexCoord2f(0.66,0.66); glVertex3f(0.0,-0.603,0.2);
  glTexCoord2f(0.66,0.33); glVertex3f(0.0,-0.603,-0.2);
  glEnd();
  glDisable(GL_TEXTURE_2D);

  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texID[3]);
  glBegin(GL_QUADS);
  glTexCoord2f(0.66,0.66); glVertex3f(-0.4,-1.8,-0.2);
  glTexCoord2f(0.66,1.0); glVertex3f(-0.4,-1.8,0.2);
  glTexCoord2f(1.0,1.0); glVertex3f(0.0,-1.8,0.2);
  glTexCoord2f(1.0,0.66); glVertex3f(0.0,-1.8,-0.2);
  glEnd();
  glDisable(GL_TEXTURE_2D);
  
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texID[0]);
  glBegin(GL_QUADS);
  glTexCoord2f(0.33,0.0); glVertex3f(0.0,-1.8,-0.2);
  glTexCoord2f(0.66,0.0); glVertex3f(0.0,-1.8,0.2);
  glTexCoord2f(0.66,1.0); glVertex3f(0.0,-0.6,0.2);
  glTexCoord2f(0.33,1.0); glVertex3f(0.0,-0.6,-0.2);
  glEnd();
  glDisable(GL_TEXTURE_2D);

  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texID[0]);
  glBegin(GL_QUADS);
  glTexCoord2f(0.33,0.0); glVertex3f(-0.4,-1.8,-0.2);
  glTexCoord2f(0.66,0.0); glVertex3f(-0.4,-1.8,0.2);
  glTexCoord2f(0.66,1.0); glVertex3f(-0.4,-0.6,0.2);
  glTexCoord2f(0.33,1.0); glVertex3f(-0.4,-0.6,-0.2);
  glEnd();
  glDisable(GL_TEXTURE_2D);

  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texID[0]);
  glBegin(GL_QUADS);
  glTexCoord2f(0.33,0.0); glVertex3f(-0.4,-1.8,0.2);
  glTexCoord2f(0.66,0.0); glVertex3f(0.0,-1.8,0.2);
  glTexCoord2f(0.66,1.0); glVertex3f(0.0,-0.6,0.2);
  glTexCoord2f(0.33,1.0); glVertex3f(-0.4,-0.6,0.2);
  glEnd();
  glDisable(GL_TEXTURE_2D);

  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texID[0]);
  glBegin(GL_QUADS);
  glTexCoord2f(0.33,0.0); glVertex3f(-0.4,-1.8,-0.2);
  glTexCoord2f(0.66,0.0); glVertex3f(0.0,-1.8,-0.2);
  glTexCoord2f(0.66,1.0); glVertex3f(0.0,-0.6,-0.2);
  glTexCoord2f(0.33,1.0); glVertex3f(-0.4,-0.6,-0.2);
  glEnd();
  glDisable(GL_TEXTURE_2D);
  glPopMatrix();
  
  //左腕
  glPushMatrix();
  glTranslatef(0.6,0.4,0.0);
  if(angle<60)
    glRotatef(30.0*sin(walk), 1.0, 0.0, 0.0);
  else
    glRotatef(90*sin(arm),0.0,0.0,1.0);
  glTranslatef(-0.6,-0.4,0.0);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texID[3]);
  glBegin(GL_QUADS);
  glTexCoord2f(0.33,0.0); glVertex3f(0.4,0.6,-0.2);
  glTexCoord2f(0.33,0.33); glVertex3f(0.4,0.6,0.2);
  glTexCoord2f(0.66,0.33); glVertex3f(0.8,0.6,0.2);
  glTexCoord2f(0.66,0.0); glVertex3f(0.8,0.6,-0.2);
  glEnd();
  glDisable(GL_TEXTURE_2D);

  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texID[3]);
  glBegin(GL_QUADS);
  glTexCoord2f(0.0,0.0); glVertex3f(0.4,-0.6,-0.2);
  glTexCoord2f(0.0,0.33); glVertex3f(0.4,-0.6,0.2);
  glTexCoord2f(0.33,0.33); glVertex3f(0.8,-0.6,0.2);
  glTexCoord2f(0.33,0.0); glVertex3f(0.8,-0.6,-0.2);
  glEnd();
  glDisable(GL_TEXTURE_2D);
  
   glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texID[0]);
  glBegin(GL_QUADS);
  glTexCoord2f(0.0,0.0); glVertex3f(0.8,-0.6,-0.2);
  glTexCoord2f(0.34,0.0); glVertex3f(0.8,-0.6,0.2);
  glTexCoord2f(0.34,1.0); glVertex3f(0.8,0.6,0.2);
  glTexCoord2f(0.0,1.0); glVertex3f(0.8,0.6,-0.2);
  glEnd();
  glDisable(GL_TEXTURE_2D);

  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texID[0]);
  glBegin(GL_QUADS);
  glTexCoord2f(0.0,0.0); glVertex3f(0.4,-0.6,-0.2);
  glTexCoord2f(0.34,0.0); glVertex3f(0.4,-0.6,0.2);
  glTexCoord2f(0.34,1.0); glVertex3f(0.4,0.6,0.2);
  glTexCoord2f(0.0,1.0); glVertex3f(0.4,0.6,-0.2);
  glEnd();
  glDisable(GL_TEXTURE_2D);

  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texID[0]);
  glBegin(GL_QUADS);
  glTexCoord2f(0.0,0.0); glVertex3f(0.4,-0.6,0.2);
  glTexCoord2f(0.34,0.0); glVertex3f(0.8,-0.6,0.2);
  glTexCoord2f(0.34,1.0); glVertex3f(0.8,0.6,0.2);
  glTexCoord2f(0.0,1.0); glVertex3f(0.4,0.6,0.2);
  glEnd();
  glDisable(GL_TEXTURE_2D);

    glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texID[0]);
  glBegin(GL_QUADS);
  glTexCoord2f(0.0,0.0); glVertex3f(0.4,-0.6,-0.2);
  glTexCoord2f(0.34,0.0); glVertex3f(0.8,-0.6,-0.2);
  glTexCoord2f(0.34,1.0); glVertex3f(0.8,0.6,-0.2);
  glTexCoord2f(0.0,1.0); glVertex3f(0.4,0.6,-0.2);
  glEnd();
  glDisable(GL_TEXTURE_2D);
  glPopMatrix();

  
  //右腕
  glPushMatrix();
  glTranslatef(-0.6,0.4,0.0);
  if(angle<60)
    glRotatef(-30.0*sin(walk), 1.0, 0.0, 0.0);
  else
    glRotatef(-90.0*sin(arm),0.0,0.0,1.0);
  glTranslatef(0.6,-0.4,0.0);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texID[3]);
  glBegin(GL_QUADS);
  glTexCoord2f(0.33,0.0); glVertex3f(-0.4,0.6,-0.2);
  glTexCoord2f(0.33,0.33); glVertex3f(-0.4,0.6,0.2);
  glTexCoord2f(0.66,0.33); glVertex3f(-0.8,0.6,0.2);
  glTexCoord2f(0.66,0.0); glVertex3f(-0.8,0.6,-0.2);
  glEnd();
  glDisable(GL_TEXTURE_2D);

  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texID[3]);
  glBegin(GL_QUADS);
  glTexCoord2f(0.0,0.0); glVertex3f(-0.4,-0.6,-0.2);
  glTexCoord2f(0.0,0.33); glVertex3f(-0.4,-0.6,0.2);
  glTexCoord2f(0.33,0.33); glVertex3f(-0.8,-0.6,0.2);
  glTexCoord2f(0.33,0.0); glVertex3f(-0.8,-0.6,-0.2);
  glEnd();
  glDisable(GL_TEXTURE_2D);
  
   glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texID[0]);
  glBegin(GL_QUADS);
  glTexCoord2f(0.0,0.0); glVertex3f(-0.8,-0.6,-0.2);
  glTexCoord2f(0.34,0.0); glVertex3f(-0.8,-0.6,0.2);
  glTexCoord2f(0.34,1.0); glVertex3f(-0.8,0.6,0.2);
  glTexCoord2f(0.0,1.0); glVertex3f(-0.8,0.6,-0.2);
  glEnd();
  glDisable(GL_TEXTURE_2D);

  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texID[0]);
  glBegin(GL_QUADS);
  glTexCoord2f(0.0,0.0); glVertex3f(-0.4,-0.6,-0.2);
  glTexCoord2f(0.34,0.0); glVertex3f(-0.4,-0.6,0.2);
  glTexCoord2f(0.34,1.0); glVertex3f(-0.4,0.6,0.2);
  glTexCoord2f(0.0,1.0); glVertex3f(-0.4,0.6,-0.2);
  glEnd();
  glDisable(GL_TEXTURE_2D);

  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texID[0]);
  glBegin(GL_QUADS);
  glTexCoord2f(0.0,0.0); glVertex3f(-0.4,-0.6,0.2);
  glTexCoord2f(0.34,0.0); glVertex3f(-0.8,-0.6,0.2);
  glTexCoord2f(0.34,1.0); glVertex3f(-0.8,0.6,0.2);
  glTexCoord2f(0.0,1.0); glVertex3f(-0.4,0.6,0.2);
  glEnd();
  glDisable(GL_TEXTURE_2D);


    glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texID[0]);
  glBegin(GL_QUADS);
  glTexCoord2f(0.0,0.0); glVertex3f(-0.4,-0.6,-0.2);
  glTexCoord2f(0.34,0.0); glVertex3f(-0.8,-0.6,-0.2);
  glTexCoord2f(0.34,1.0); glVertex3f(-0.8,0.6,-0.2);
  glTexCoord2f(0.0,1.0); glVertex3f(-0.4,0.6,-0.2);
  glEnd();
  glDisable(GL_TEXTURE_2D);
  glPopMatrix();
  glPopMatrix();
    
  //難易度表示
  if(angle>208){
    if(angle<230){
      glEnable(GL_TEXTURE_2D);
      glBindTexture(GL_TEXTURE_2D, texID[11+nn]);
      glBegin(GL_QUADS);
      glTexCoord2f(0.0,0.0); glVertex3f(-1.2,pointy-0.64,pointz-2-kk);
      glTexCoord2f(1.0,0.0); glVertex3f(1.2,pointy-0.64,pointz-2-kk);
      glTexCoord2f(1.0,0.54); glVertex3f(1.2,pointy+0.64,pointz-2-kk);
      glTexCoord2f(0.0,0.54); glVertex3f(-1.2,pointy+0.64,pointz-2-kk);
      glEnd();
      glDisable(GL_TEXTURE_2D);
    }
  }
  
  //地面
  if(sic!=1){
    if(angle>200){
      glEnable(GL_TEXTURE_2D);
      glBindTexture(GL_TEXTURE_2D, texID[10]);
      glBegin(GL_QUADS);
      glTexCoord2f(0.0,0.0); glVertex3f(100,100,200);
      glTexCoord2f(1.0,0.0); glVertex3f(100,-100,200);
      glTexCoord2f(1.0,1.0); glVertex3f(-100,-100,200);
      glTexCoord2f(0.0,1.0); glVertex3f(-100,100,200);
      glEnd();
      glDisable(GL_TEXTURE_2D);
    }
  
    //○
    if(angle>212){
      glEnable(GL_TEXTURE_2D);
      glBindTexture(GL_TEXTURE_2D, texID[16]);
      glBegin(GL_QUADS);
      glTexCoord2f(0.0,1.0); glVertex3f(-0.1,-1.9,23*q);
      glTexCoord2f(1.0,1.0); glVertex3f(-0.1,2.9,23*q);
      glTexCoord2f(1.0,0.0); glVertex3f(-4.9,2.9,23*q);
      glTexCoord2f(0.0,0.0); glVertex3f(-4.9,-1.9,23*q);
      glEnd();
      glDisable(GL_TEXTURE_2D);
      
      //×
      glEnable(GL_TEXTURE_2D);
      glBindTexture(GL_TEXTURE_2D, texID[15]);
      glBegin(GL_QUADS);
      glTexCoord2f(0.0,1.0); glVertex3f(0.1,-1.9,23*q);
      glTexCoord2f(1.0,1.0); glVertex3f(0.1,2.9,23*q);
      glTexCoord2f(1.0,0.0); glVertex3f(4.9,2.9,23*q);
      glTexCoord2f(0.0,0.0); glVertex3f(4.9,-1.9,23*q);
      glEnd();
      glDisable(GL_TEXTURE_2D);
    }
  }

  //正解数
  if(sic!=1){
  if(angle>212){
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texID[sc]);
  glBegin(GL_QUADS);
  glTexCoord2f(0.0,0.0); glVertex3f(3.0,1.4,walk-8.0*a);
  glTexCoord2f(1.0,0.0); glVertex3f(2.4,1.4,walk-8.0*a);
  glTexCoord2f(1.0,1.0); glVertex3f(2.4,2.0,walk-8.0*a);
  glTexCoord2f(0.0,1.0); glVertex3f(3.0,2.0,walk-8.0*a);
  glEnd();
  glDisable(GL_TEXTURE_2D);

  //難易度
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texID[26+nn]);
  glBegin(GL_QUADS);
  glTexCoord2f(0.0,0.0); glVertex3f(3.0,0.8,walk-8.0*a);
  glTexCoord2f(1.0,0.0); glVertex3f(2.4,0.8,walk-8.0*a);
  glTexCoord2f(1.0,1.0); glVertex3f(2.4,1.4,walk-8.0*a);
  glTexCoord2f(0.0,1.0); glVertex3f(3.0,1.4,walk-8.0*a);
  glEnd();
  glDisable(GL_TEXTURE_2D);  
  }
  }
  //特別問題用のテクスチャ
  if(sic!=1){
    if(angle>222){
      if(walk<23*q-5){
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texID[29+q+zxc]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0,1.0); glVertex3f(1.0,0.0,walk-8.0);
	glTexCoord2f(1.0,1.0); glVertex3f(-1.0,0.0,walk-8.0);
	glTexCoord2f(1.0,0.0); glVertex3f(-1.0,-2.0,walk-8.0);
	glTexCoord2f(0.0,0.0); glVertex3f(1.0,-2.0,walk-8.0);
	glEnd();
	glDisable(GL_TEXTURE_2D);  
      }
    }
  }
  //最初の難易度選択メニュー
  if(sg==0){
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texID[46+op]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0,0.0); glVertex3f(-0.6,-0.6,pointz+1);
    glTexCoord2f(1.0,0.0); glVertex3f(0.6,-0.6,pointz+1);
    glTexCoord2f(1.0,1.0); glVertex3f(0.6,0.6,pointz+1);
    glTexCoord2f(0.0,1.0); glVertex3f(-0.6,0.6,pointz+1);
    glEnd();
    glDisable(GL_TEXTURE_2D);
 
  }
  //とある会社
  //一枚の画像でなく4枚の画像を組み合わせて作った
  if(sic==1){
    if(angle<315){
      glEnable(GL_TEXTURE_2D);
      glBindTexture(GL_TEXTURE_2D, texID[52]);
      glBegin(GL_QUADS);
      glTexCoord2f(0.0,0.0); glVertex3f(0.0,0.0,-108);
      glTexCoord2f(1.0,0.0); glVertex3f(2.0,0.0,-108);
      glTexCoord2f(1.0,1.0); glVertex3f(2.0,2.0,-108);
      glTexCoord2f(0.0,1.0); glVertex3f(0.0,2.0,-108);
      glEnd();
      glDisable(GL_TEXTURE_2D);

      glEnable(GL_TEXTURE_2D);
      glBindTexture(GL_TEXTURE_2D, texID[51]);
      glBegin(GL_QUADS);
      glTexCoord2f(0.0,0.0); glVertex3f(-2.0,0.0,-108);
      glTexCoord2f(1.0,0.0); glVertex3f(0.0,0.0,-108);
      glTexCoord2f(1.0,1.0); glVertex3f(0.0,2.0,-108);
      glTexCoord2f(0.0,1.0); glVertex3f(-2.0,2.0,-108);
      glEnd();
      glDisable(GL_TEXTURE_2D);
      
      glEnable(GL_TEXTURE_2D);
      glBindTexture(GL_TEXTURE_2D, texID[54]);
      glBegin(GL_QUADS);
      glTexCoord2f(0.0,0.0); glVertex3f(0.0,2.0,-108);
      glTexCoord2f(1.0,0.0); glVertex3f(2.0,2.0,-108);
      glTexCoord2f(1.0,1.0); glVertex3f(2.0,4.0,-108);
      glTexCoord2f(0.0,1.0); glVertex3f(0.0,4.0,-108);
      glEnd();
      glDisable(GL_TEXTURE_2D);
      
      glEnable(GL_TEXTURE_2D);
      glBindTexture(GL_TEXTURE_2D, texID[53]);
      glBegin(GL_QUADS);
      glTexCoord2f(0.0,0.0); glVertex3f(-2.0,2.0,-108);
      glTexCoord2f(1.0,0.0); glVertex3f(0.0,2.0,-108);
      glTexCoord2f(1.0,1.0); glVertex3f(0.0,4.0,-108);
      glTexCoord2f(0.0,1.0); glVertex3f(-2.0,4.0,-108);
      glEnd();
      glDisable(GL_TEXTURE_2D);
    }else if(sc-17<5){

      glEnable(GL_TEXTURE_2D);
      glBindTexture(GL_TEXTURE_2D, texID[52]);
      glBegin(GL_QUADS);
      glTexCoord2f(0.0,0.0); glVertex3f(0.0,0.0,-108);
      glTexCoord2f(1.0,0.0); glVertex3f(2.0,0.0,-108);
      glTexCoord2f(1.0,1.0); glVertex3f(2.0,2.0,-108);
      glTexCoord2f(0.0,1.0); glVertex3f(0.0,2.0,-108);
      glEnd();
      glDisable(GL_TEXTURE_2D);

      glEnable(GL_TEXTURE_2D);
      glBindTexture(GL_TEXTURE_2D, texID[51]);
      glBegin(GL_QUADS);
      glTexCoord2f(0.0,0.0); glVertex3f(-2.0,0.0,-108);
      glTexCoord2f(1.0,0.0); glVertex3f(0.0,0.0,-108);
      glTexCoord2f(1.0,1.0); glVertex3f(0.0,2.0,-108);
      glTexCoord2f(0.0,1.0); glVertex3f(-2.0,2.0,-108);
      glEnd();
      glDisable(GL_TEXTURE_2D);
      
      glEnable(GL_TEXTURE_2D);
      glBindTexture(GL_TEXTURE_2D, texID[54]);
      glBegin(GL_QUADS);
      glTexCoord2f(0.0,0.0); glVertex3f(0.0,2.0,-108);
      glTexCoord2f(1.0,0.0); glVertex3f(2.0,2.0,-108);
      glTexCoord2f(1.0,1.0); glVertex3f(2.0,4.0,-108);
      glTexCoord2f(0.0,1.0); glVertex3f(0.0,4.0,-108);
      glEnd();
      glDisable(GL_TEXTURE_2D);
      
      glEnable(GL_TEXTURE_2D);
      glBindTexture(GL_TEXTURE_2D, texID[53]);
      glBegin(GL_QUADS);
      glTexCoord2f(0.0,0.0); glVertex3f(-2.0,2.0,-108);
      glTexCoord2f(1.0,0.0); glVertex3f(0.0,2.0,-108);
      glTexCoord2f(1.0,1.0); glVertex3f(0.0,4.0,-108);
      glTexCoord2f(0.0,1.0); glVertex3f(-2.0,4.0,-108);
      glEnd();
      glDisable(GL_TEXTURE_2D);
    }
    
    //最終画面のコンクリートの地面
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texID[55]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0,0.0); glVertex3f(-100.0,0,-150);
    glTexCoord2f(1.0,0.0); glVertex3f(100.0,0,-150);
    glTexCoord2f(1.0,1.0); glVertex3f(100.0,0,-50);
    glTexCoord2f(0.0,1.0); glVertex3f(-100.0,0,-50);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    /*ここでは最終画面の成功か失敗かを表示するものでテクスチャの読み込めるピクセルの関係上一つの
      画像だけで表示しようとすると荒くなってしまうので2枚の画像を組み合わせて作った*/
    //失敗
    if(sc-17<5){
      if(angle>313){
      glEnable(GL_TEXTURE_2D);
      glBindTexture(GL_TEXTURE_2D, texID[56]);
      glBegin(GL_QUADS);
      glTexCoord2f(0.0,0.0); glVertex3f(-2.0,2.0,-104);
      glTexCoord2f(0.0,0.5); glVertex3f(-2.0,4.0,-106);
      glTexCoord2f(1.0,0.5); glVertex3f(0.0,4.0,-106);
      glTexCoord2f(1.0,0.0); glVertex3f(0.0,2.0,-104);
      glEnd();
      glDisable(GL_TEXTURE_2D);

      glEnable(GL_TEXTURE_2D);
      glBindTexture(GL_TEXTURE_2D, texID[57]);
      glBegin(GL_QUADS);
      glTexCoord2f(0.0,0.0); glVertex3f(0.0,2.0,-104);
      glTexCoord2f(0.0,0.5); glVertex3f(0.0,4.0,-106);
      glTexCoord2f(1.0,0.5); glVertex3f(2.0,4.0,-106);
      glTexCoord2f(1.0,0.0); glVertex3f(2.0,2.0,-104);
      glEnd();
      glDisable(GL_TEXTURE_2D);
      }
    }else{
      //成功
      if(angle>318){
      glEnable(GL_TEXTURE_2D);
      glBindTexture(GL_TEXTURE_2D, texID[56]);
      glBegin(GL_QUADS);
      glTexCoord2f(0.0,0.5); glVertex3f(-2.0,2.0,-104);
      glTexCoord2f(0.0,1.0); glVertex3f(-2.0,4.0,-106);
      glTexCoord2f(1.0,1.0); glVertex3f(0.0,4.0,-106);
      glTexCoord2f(1.0,0.5); glVertex3f(0.0,2.0,-104);
      glEnd();
      glDisable(GL_TEXTURE_2D);

      glEnable(GL_TEXTURE_2D);
      glBindTexture(GL_TEXTURE_2D, texID[57]);
      glBegin(GL_QUADS);
      glTexCoord2f(0.0,0.5); glVertex3f(0.0,2.0,-104);
      glTexCoord2f(0.0,1.0); glVertex3f(0.0,4.0,-106);
      glTexCoord2f(1.0,1.0); glVertex3f(2.0,4.0,-106);
      glTexCoord2f(1.0,0.5); glVertex3f(2.0,2.0,-104);
      glEnd();
      glDisable(GL_TEXTURE_2D);
      }
  }
  }

  glDisable(GL_NORMALIZE );
  
  glPopMatrix();
  glDisable( GL_LIGHTING );
  glDisable( GL_DEPTH_TEST );	

  
}

//爆発を表現するためのCGで材質はなるべく光を反射しないような設定にした
void Drawg(void){
  GLfloat material_specular[4] = { 0.7f, 0.04f, 0.04f, 1.0f };
  GLfloat material_diffuse[4] = { 0.5f, 0.4f, 0.4f, 1.0f };
  GLfloat material_ambient[4] = { 0.05f, 0.0f, 0.0f, 1.0f };
  
  glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambient);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);
  glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular);
  glMaterialf(GL_FRONT, GL_SHININESS, 10.0);

  if(angle<315){
  glPushMatrix();
  glColor3f(1.0, 0.0, 0.0);
  glTranslatef(0.0, 0.0, -110.0);
  glutSolidSphere(ss, 20, 20);
  glPopMatrix();
  }

}

//通常のCGはここで表現する
//ヘリは反射するような材質の設定にした
void DrawHeli(void){
  float diffuse[] = { 0.752, 0.752, 0.752, 1.0 };
  float specular[] = { 0.508, 0.508, 0.508, 1.0 };
  float ambient[] = { 0.192, 0.192, 0.192, 1.0 };
  float ambient_light[] = { 2.5, 2.5, 2.5, 1.0 };
  
  glMaterialfv( GL_FRONT, GL_DIFFUSE, diffuse );
  glMaterialfv( GL_FRONT, GL_SPECULAR, specular );
  glMaterialfv( GL_FRONT, GL_AMBIENT, ambient );
  glMaterialf( GL_FRONT, GL_SHININESS, 51.2 );

  glLightfv( GL_LIGHT0, GL_DIFFUSE,  diffuse);
  glLightfv( GL_LIGHT0, GL_SPECULAR, specular);
  glLightfv( GL_LIGHT0, GL_AMBIENT, ambient_light);
  
  glEnable( GL_DEPTH_TEST );
  
  glEnable(GL_NORMALIZE);

  glEnable( GL_LIGHTING );
  
   //ヘリ
  if(angle<190){
    glPushMatrix();
    glTranslatef(0.0,-1.8,4.5);
    glScalef(40, 1.0,150);
    glutSolidCube(0.1);
    glPopMatrix();
    
      glPushMatrix();
      glTranslatef(2.0,0.1,4.5);
      glScalef(1.0, 40,150);
      glutSolidCube(0.1);
      glPopMatrix();
      
      glPushMatrix();
      glTranslatef(-2.0,0.1,4.5);
      glScalef(1.0, 40,150);
      glutSolidCube(0.1);
      glPopMatrix();
      
      glPushMatrix();
      glTranslatef(0.0,2.0,4.5);
      glScalef(40,1.0,150);
      glutSolidCube(0.1);
      glPopMatrix();
      
      glPushMatrix();
      glTranslatef(0.0,0.1,-3.0);
      glScalef(40,40,1.0);
      glutSolidCube(0.1);
      glPopMatrix();
      
      
      glPushMatrix();
      glTranslatef(0.0,0.1,12.0);
      glTranslatef(0.0,-1.8,0.0);
      glRotatef(45*(cos(door)+1),1.0,0.0,0.0);
      glTranslatef(0.0,1.8,0);
      glScalef(40,40,1.0);
      glutSolidCube(0.1);
      glPopMatrix();
      
      glPushMatrix();
      glTranslatef(0.0,2.2,0.0);
      glRotatef((angle*100),0.0,1.0,0.0);
      glScalef(200,2,8.0);
      glutSolidCube(0.1);
      glPopMatrix();

      glPushMatrix();
      glTranslatef(0.0,2.2,9.0);
      glRotatef((angle*100),0.0,1.0,0.0);
      glScalef(200,2,8.0);
      glutSolidCube(0.1);
      glPopMatrix();  
  }

  glDisable(GL_NORMALIZE );

  glDisable( GL_LIGHTING );
  glDisable( GL_DEPTH_TEST );

  }
//  描画のたびに呼び出される関数 
void OurDisplay(void)
{

  if(angle>220){
    if(walk<200){
	//  スムーズシェイディングを指定
	glShadeModel(GL_FLAT);
    }
  }

	//  画面とデプスバッファを消去する
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_VIEWPORT);
	glLoadIdentity();
	float aspect;

	  aspect = (float) 960 / (float) 640;
	  glViewport(0, 0, 960, 640);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fovy, aspect, nearw, farw);
	if(sg==1){
	  if(lookz>=-4.0){
	    if(angle<20)
	      lookz=lookz-close;
	  }
	}
	gluLookAt(lookx,looky,lookz, pointx, pointy, pointz, 0.0,1.0, 0.0);
	  
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// --- humanを描画する --- //
	InitLight();
	DrawHuman();
	Drawg();
	DrawHeli();

	glFlush();

	//  ダブルバッファのバッファを入れ替える
	glutSwapBuffers();
}

//キーボードを押すことにより発生するイベントを設定する関数
void myKbd(unsigned char key, int x, int y)
{
  switch(key) {
  case KEY_ESC:
    exit( 0 );//ESCを押したら終了

  case KEY_W:
    if(sg==0){
      if(op!=0)
	op=op-1;
      else
	op=4;
    }
    break;
  case KEY_S:
    if(sg==0){
      if(op!=4)
	op=op+1;
      else
	op=0;
    }
    break;
  case KEY_SPACE:
    if(sic==0){
      if(sg==0){
	if(op==4)
	  exit(0);
	else{
	  nn=op;
	  sg=1;
	  if(op==3)
	    zxc=8;
	}
      }
    }else if(sic==1){
      if(angle>319)
	sic=2;
    }
    break;
  default:
    break;
  }
}

//テクスチャを読み込む関数
void initTexturePPM(void)
{
  glGenTextures(NumOfTextureFiles, (unsigned int *)texID);
  
  //  テクスチャデータを読み込む関数を実行する 
  int i; 
  for(i=0; i<NumOfTextureFiles; i++){
    LoadASCIIPPMImage(images[i]);
    
    glBindTexture(GL_TEXTURE_2D, texID[i]); 
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, imageWidth, imageHeight, 0,GL_RGB, GL_UNSIGNED_BYTE, &image);
  }
  
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    
    }

//初期設定を行う関数
void myInit (char *progname)
{
  glutInitWindowPosition(0, 0);
  glutInitWindowSize(960, 640);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
  glutCreateWindow(progname);
  glClearColor (0.6, 0.96, 1, 1);
  //glutKeyboardFunc(myKbd);
  
  resetview();
  initTexturePPM();
  
  glShadeModel( GL_SMOOTH );
  glEnable( GL_LIGHT0 );
}

//グローバル座標に関する設定を行う関数
void polarview( void )
{
  glTranslatef( 0.0, 0.0, -distance);
  glRotatef( -twist, 0.0, 0.0, 1.0);
  glRotatef( -elevation, 1.0, 0.0, 0.0);
  glRotatef( -azimuth, 0.0, 1.0, 0.0);
}

//変数を初期化する関数
void resetview( void )
{
  Signal=1;
  sg=0;
  op=0;
  q=1;
  nearw=2.0;
  distance = 0.0;
  twist = 0.0;
  elevation = 0.0;
  azimuth = 0.0;
  lookx=0;
  looky=2.0;
  lookz=4.0;
  pointx=0;
  pointy=0;
  pointz=0;
  trans=0;
  angle=0;
  door=PI2/2;
  close=0;
  walk=0;
  arm=0;
  jumpx=0;
  jumpy=0;
  ball=0;
  lost=0;
  kk=0;
  plus=0;
  ss=0;
  a=1;
  sc=17;
  nn=0;
  zxc=0;
  sic=0;
}

//マウスの動作を保存する関数
void myMouse( int button, int state, int x, int y )
{
  if( state == GLUT_DOWN) {
    mButton = button;
    xBegin = x;
    yBegin = y;
  }
}

//マウスの動きから光源や物体を動かしたりする関数
void myMotion(int x, int y)
{
  int xDisp, yDisp;
  
  xDisp = x - xBegin;
  yDisp = y - yBegin;
  switch(mButton){
  case GLUT_LEFT_BUTTON:
    if(angle>227){
      if(sic!=1){
      jumpx -= (float) xDisp/50.0;
      jumpy -= (float) yDisp/50.0;
    }
    }
    break;
  }
  xBegin = x;
  yBegin = y;
  glutPostRedisplay();
}

void ourIdle(void)
{

  if(sg>0){
	//  アニメーションの設定
	angle = angle + 0.1;

	if(angle>20){
	  if(door>0)
	    door = door - 0.02;
	}

	if(angle>5)
	close = close + 0.005;

	if(angle>40){
	  if(angle<60){
	    if(walk<=15.7){
	      walk =walk+0.1;
	      lookz =lookz+0.1;
	      pointz =pointz+0.1;
	    }
	  }
	}

	if(angle>60){
	  if(angle<70){
	    if(arm<1.58)
	      arm=arm+0.02;
	  }
	}

	if(angle>70){
	  if(angle<110){
	    if(lookz<30.8)
	      lookz=lookz+0.1;
	    if(looky>-4)
	      looky=looky-0.06;
	  }
	}
	
	if(angle>110){
	  if(angle<190){
	    walk=walk+0.01;
	    lookz=lookz+0.01;
	    pointz=pointz+0.01;
	    ball=ball+0.01;
	    jumpy=-(ball-0.5)*(ball-0.5)+1;
	    looky=jumpy-4;
	    pointy=jumpy;
	  }
	}

	if(angle>190){
	  if(angle<215)
	    lookz=lookz-0.1;
	}
	if(angle>200){
	  if(angle<204)
	     looky=looky+0.1;
	     }

	if(angle>220)
	  kk=kk+0.5;

	if(angle>221){
	  if(angle<222){
	    lookx=0.0;
	    looky=0.0;
	    lookz=-11.5;
	    pointx=0.0;
	    pointy=0.0;
	    pointz=0.0;
	    jumpx=0.0;
	    jumpy=0.0;
	    walk=0.0;
	    nearw=0.05;
	  }
	}

	if(angle>222){
	  if(sic!=1){
	    if(walk<200){
	      if(nn!=3){
		lookz=lookz+0.1+0.1*nn;
		pointz=pointz+0.1+0.1*nn;
		walk=walk+0.1+0.1*nn;
	      }else{
		lookz=lookz+0.1;
		pointz=pointz+0.1;
		walk=walk+0.1;
	      }
	    }
	  }
	}

	if(walk-4>23*q){
	  if(q<8)
	  q=q+1;
	}
	if(nn!=3){
	  if(walk>23*q){
	    if(walk<23*q+0.1+0.1*nn){
	      switch(q){
	      case 1:
	      case 3:
	      case 4:
	      case 5:
		if(jumpx<-0.1){
		  if(jumpx>-4.9){
		    if(jumpy<2.9){
		      if(jumpy>-1.9){
			sc=sc+1;
		      }
		    }
		  }
		}
		break;
	      case 2:
	      case 6:
	      case 7:
	      case 8:
		if(jumpx>0.1){
		  if(jumpx<4.9){
		    if(jumpy<2.9){
		      if(jumpy>-1.9){
			sc=sc+1;
		      }
		    }
		  }
		}
		break;
	      default:
		break;
	      }
	    }
	  }
	}else{
	  if(walk>23*q){
	    if(walk<23*q+0.1){
	      switch(q){
	      case 2:
	      case 3:
	      case 4:
	      case 7:
		if(jumpx<-0.1){
		  if(jumpx>-4.9){
		    if(jumpy<2.9){
		      if(jumpy>-1.9){
			sc=sc+1;
 		      }
		    }
		  }
		}
		break;
	      case 1:
	      case 5:
	      case 6:
	      case 8:
		if(jumpx>0.1){
		  if(jumpx<4.9){
		    if(jumpy<2.9){
		      if(jumpy>-1.9){
			sc=sc+1;
		      }
		    }
		  }
		}
		break;
	      default:
		break;
	      }
	    }
	  }
	}
	if(walk>198){
	  if(walk<200){
	  lookz=-100;
	  looky=4;
	  lookx=0;
	  pointx=0.0;
	  pointy=1.0;
	  pointz=-110;
	  jumpx=0.0;
	  jumpy=20;
	  walk=-130;
	  angle=300;
	  sic=1;
	  nearw=2.0;
	  a=-0.8125;
	  }
	}
	
	if(sic==1){
	  if(angle>305){
	    jumpy=jumpy-0.5;
	  }
	  if(angle>310){
	    if(sc-17>=5)
	      ss=ss+0.3;
	  }
	}
	
  }
  if(sic==2){
    glutIdleFunc(resetview);

  }
    
        //  強制的に再描画する 
	glutPostRedisplay();
}

//プログラムを動かす上でメインとなる関数
void main(int argc, char** argv)
{
  glutInit(&argc, argv);
  //  ダブルバッファを用意
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  myInit(argv[0]);
  glutIdleFunc(ourIdle);
  glutDisplayFunc(OurDisplay);
  glutMouseFunc(myMouse);
  glutMotionFunc( myMotion );
  glutKeyboardFunc(myKbd);
  //  デプスバッファを使うように設定
  glEnable(GL_DEPTH_TEST);
  glutMainLoop();
}

/*  --- 以降，テクスチャファイルを読み込むために必要な関数 ---  */
char* get_word(void)
{
  while(1)
    {
      if(*ptr == '\0')
	{
	  fgets(readbuf, 256, texturefile);
	  while(*readbuf == '#')
	    fgets(readbuf, 256, texturefile);
	  ptr = readbuf;
	  continue;
	}
      else if(isspace(*ptr))
	{
	  ptr++;
	  continue;
	}
      else
	break;
    }
  
  ptr2 = word;
  while(!isspace(*ptr))
    *ptr2++ = *ptr++;
*ptr2 = '\0';
return word;
}

void LoadASCIIPPMImage(const char* filename)
{
  int i, j;
  
  texturefile = fopen(filename, "r");
  
  get_word();
  
  for (i = imageHeight-1; i >=0 ; i--) 
    {
      for (j = 0; j < imageWidth; j++) 
	{
	  image[i][j][0] = (GLubyte) atoi(get_word());
	  image[i][j][1] = (GLubyte) atoi(get_word());
	  image[i][j][2] = (GLubyte) atoi(get_word());
	}
    }
  fclose(texturefile);
}

//円柱を作るための関数
void mySolidCylinder( float r, float h, int n )
{
  float x, y, z, dq;
  int i;
  
  glEnable( GL_NORMALIZE );
  dq = PI2/(float)n;
  y = 0.5*h;
  glPushMatrix();
  glRotatef( -dq*180.0/PI2, 0.0, 0.1, 0.0 );
  glBegin( GL_QUAD_STRIP ); 
  for( i=0; i<=n; i+=1 ){
    x = r*cos( dq*(float)i );
    z = r*sin( dq*(float)i );
    glNormal3f( x,  0, z ); 
    glVertex3f( x,  y, z );
    glVertex3f( x, -y, z );
  }
  glEnd();
  glBegin( GL_POLYGON );
  glNormal3f( 0.0, -1.0, 0.0 );
  for( i=0; i<n; i+=1 ){
    x = r*cos( dq*(float)i );
    z = r*sin( dq*(float)i );
    glVertex3f( x, -y, z );
  }
  glEnd();
  glBegin( GL_POLYGON ); 
  glNormal3f( 0.0, 1.0, 0.0 ); 
  for( i=0; i<n; i+=1 ){	
    
    x = r*cos( dq*(float)i );
    z = r*sin( dq*(float)i );
    glVertex3f( x, y, z );
  }
  glEnd();
  glPopMatrix();
  glDisable( GL_NORMALIZE );
}

