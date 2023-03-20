/* programa labirinto.c */

#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>

typedef int bool;
#define true 1
#define false 0

/* Directions */
#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4

/* Screens */
#define TITLE_SCREEN 1
#define DIFFICULT_SCREEN 2
#define GAME_SCREEN 3
#define VICTORY_SCREEN 4
#define GAMEOVER_SCREEN 5

struct place {
  bool up;
  bool down;
  bool left;
  bool right;
  bool start;
  bool end;
  bool hero;
  int x; //point on screen to draw the hero
  int y;
  bool footprint;
};

// prototipos das funcoes
void init(void);
void display(void);
void keyboard(unsigned char key, int x, int y);
void createMaze (void);
void destroyMaze (void);
void output(int, int, float, float, float, char *);
bool canHeroGo (int);
int whereIsHeroC (void);
int whereIsHeroR (void);
void mazeGenerator (void);
char* itoa(int, char[]);
void doAnWay (void);
void timeCount (int);
//int ** maze;
struct place ** maze;
int row, column;
int heroX, heroY;
int heroC, heroR;
int startC, startR, endC, endR;
int windowSizeX, windowSizeY;
int borderX, borderY;
int steps = 0;
bool gameover = false;
long int seed;
int timeLeft;
int screen;
int mazeProbability;

// funcao principal
int main(int argc, char** argv){
  //srand (time (NULL));
  screen = TITLE_SCREEN;
  seed = time (NULL);
  mazeProbability = 3;
//  seed = 100;
  srand (seed);
  timeLeft = 30;
  //printf ("Digite o tamanho do labirinto\n");
  //scanf ("%d %d", &row, &column);
  row = 60;
  column = 60;
  printf ("Construindo um labirinto de %d x %d\n", row, column);

  windowSizeX = 1000;//100 + row * 10;
  windowSizeY = 1000;// + column * 10;
  borderX = 50;
  borderY = 50;

  //heroC = rand() % (column / 2); //make the hero start at left side
  heroC = 0;
  //heroR = row / 2; //if I put row / 2 both to heroR and endR, the pionner cannot open a way
  heroR = rand() % row;
  //heroR = (rand() % row / 2) + (row / 2);
  startC = heroC;
  startR = heroR;
  do {
    //endC = (rand() % (column /2)) + (column / 2); //make the end point be at right sideda
    endC = column - 1;
    //endR = row / 2;
    //endR = (rand() % row / 2);
    endR = rand() % row;
  } while (startC == endC || startR == endR);

  printf ("heroi em %d %d\n", heroR, heroC);
  printf ("start em %d %d\n", startR, startC);
  printf ("end em %d %d\n", endR, endC);

  glutInit(&argc, argv);                              // start glut
  glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);           // especifies the colors and buffers
  glutInitWindowSize (windowSizeX, windowSizeY);             // specifies the window size
  glutInitWindowPosition (10, 10);                      // specifies where to show the window
  glutCreateWindow ("Desenhando uma linha");               // create window
  createMaze ();
  init();
  glutDisplayFunc(display);                               // function to redraw using GLUT
  glutKeyboardFunc(keyboard);                             // keyboard function
  glutTimerFunc(1000,timeCount,1);
  glutMainLoop();                                         // show all created window

  return 0;
}



void init(void){
  glClearColor(1.0, 1.0, 1.0, 1.0);    // cor de fundo
  glOrtho (0, (borderX + (row * 10)), 0, (borderY + (column * 10)), -1 ,1);     // modo de projecao ortogonal
} 

void display(void){
  glClear(GL_COLOR_BUFFER_BIT);               // limpa a janela
  


    /* A small point means a place center */
    int shiftX = borderX / 2;
    int shiftY = borderY / 2;
    int x,y,i,j;
    glPointSize (1.0);
    for (i = 0; i < row; i++) {
      x = ((i + 1) * 10) + shiftX;
      for (j = 0; j < column; j++) {
        y = ((j + 1) * 10) + shiftY;

        maze[i][j].x = x;
        maze[i][j].y = y;

        glPointSize (1.0);
        glColor3f (0.0, 0.0, 0.0);
        glBegin (GL_POINTS);
          glVertex2i (x, y);
        glEnd();

        /* Draw footprints */
        if (maze[i][j].footprint == true) {
          glPointSize (15.0);
          glColor3f (1.0, 1.0, 0.0);
          glBegin(GL_POINTS);
            glVertex2i (maze[i][j].x, maze[i][j].y);
          glEnd();        
        }

        /* Draw start point */
        if (maze[i][j].start == true) {
          glPointSize (10.0);
          glColor3f (0.0, 1.0, 0.0);
          glBegin(GL_POINTS);
            glVertex2i (maze[i][j].x, maze[i][j].y);
          glEnd();        
        }
      
        /* Draw end point */
        if (maze[i][j].end == true) {
          glPointSize (10.0);
          glColor3f (1.0, 0.0, 0.0);
          glBegin(GL_POINTS);
            glVertex2i (maze[i][j].x, maze[i][j].y);
          glEnd();        
        }
  
        /* Draw hero */
        if (maze[i][j].hero == true) {
          printf ("é verdade i = %d e j = %d\n", i, j);
          glPointSize (10.0);
          glColor3f (0.0, 0.0, 1.0);
          glBegin(GL_POINTS);
            glVertex2i (maze[i][j].x, maze[i][j].y);
          glEnd();        
          printf ("hero está em %d %d\n",i,j);
          printf ("hero é desenhado em %d %d\n",maze[i][j].x,maze[i][j].y);
        }

        /* Construindo das paredes */
        glPointSize (1.0);
        glColor3f (0.0, 0.0, 0.0);
        if (maze[i][j].up == false) {
          glBegin (GL_LINES);
            glVertex2i ((maze[i][j].x-5),(maze[i][j].y+5));
            glVertex2i ((maze[i][j].x+5),(maze[i][j].y+5));
          glEnd ();
        }
        if (maze[i][j].down == false) {
          glBegin (GL_LINES);
            glVertex2i ((maze[i][j].x-5),(maze[i][j].y-5));
            glVertex2i ((maze[i][j].x+5),(maze[i][j].y-5));
          glEnd ();
        }
        if (maze[i][j].left == false) {
          glBegin (GL_LINES);
            glVertex2i ((maze[i][j].x-5),(maze[i][j].y+5));
            glVertex2i ((maze[i][j].x-5),(maze[i][j].y-5));
          glEnd ();
        }
        if (maze[i][j].right == false) {
          glBegin (GL_LINES);
            glVertex2i ((maze[i][j].x+5),(maze[i][j].y+5));
            glVertex2i ((maze[i][j].x+5),(maze[i][j].y-5));
          glEnd ();
        }
      }
    }

    char *str = "Maze";
    output(300, 640, 0.0, 0.0, 0.0, str);

    char str2[30];
    itoa(steps, str2);
    output(200, 10, 0.0, 0.0, 0.0, strcat(str2, " steps"));

    char str4[30] = "timeLeft: ";
    char str5[10];
    itoa(timeLeft, str5);
    output(400, 10, 0.0, 0.0, 0.0, strcat(str4, str5));
  
    if (screen == VICTORY_SCREEN) {
      glColor3f(0.0, 0.0, 0.0);
      glBegin(GL_POLYGON);
        glVertex2i(250, 330);
        glVertex2i(460, 330);
        glVertex2i(460, 260);
        glVertex2i(250, 260);
      glEnd();
      char *str3 = "CONGRATULATIONS!";
      output(300, 310, 1.0, 1.0, 1.0, str3);
      char str4[100] = "YOU ESCAPED WITH ";
      char str5[10];
      itoa (steps, str5);
      char str6[10] = " STEPS!";
      strcat (str4, str5);
      strcat (str4, str6);
      output(260, 290, 1.0, 1.0, 1.0, str4);
      char *str7 = "PRESS ESC TO EXIT!";
      output(300, 270, 1.0, 1.0, 1.0, str7);


    itoa(steps, str2);
    output(200, 10, 0.0, 0.0, 0.0, strcat(str2, " steps"));
    }

    if (screen == TITLE_SCREEN) {
      glColor3f(0.0, 0.0, 0.0);
      glBegin(GL_POLYGON);
        glVertex2i(250, 420);
        glVertex2i(440, 420);
        glVertex2i(440, 260);
        glVertex2i(250, 260);
      glEnd();
      char *str3 = "RUN TO THE HILL!";
      output(260, 400, 1.0, 1.0, 1.0, str3);
      char *str4 = "RUN FOR YOUR LIFE!";
      output(260, 380, 1.0, 1.0, 1.0, str4);
      char *str5 = "CHOOSE DIFFICULTY";
      output(260, 350, 1.0, 1.0, 1.0, str5);
      char *str6 = "PRESS 1 TO EASY MODE!";
      output(260, 330, 1.0, 1.0, 1.0, str6);
      char *str7 = "PRESS 2 TO NORMAL MODE!";
      output(260, 310, 1.0, 1.0, 1.0, str7);
      char *str8 = "PRESS 3 TO HARD MODE!";
      output(260, 290, 1.0, 1.0, 1.0, str8);
      char *str9 = "PRESS 4 TO GOD MODE!";
      output(260, 270, 1.0, 1.0, 1.0, str9);
    }    
    
    if (screen == GAMEOVER_SCREEN) {
      glColor3f(0.0, 0.0, 0.0);
      glBegin(GL_POLYGON);
        glVertex2i(290, 330);
        glVertex2i(430, 330);
        glVertex2i(430, 280);
        glVertex2i(290, 280);
      glEnd();
      char *str3 = "GAMEOVER!";
      output(325, 310, 1.0, 1.0, 1.0, str3);
      char *str4 = "PRESS ESC TO EXIT!";
      output(300, 290, 1.0, 1.0, 1.0, str4);
    }    


    glFlush();

}

bool canHeroGo (int direction) {
  bool can = true;
  int c = whereIsHeroC ();
  int r = whereIsHeroR ();
  
  if (direction == UP && maze[c][r].up == false)
    can = false;
  if (direction == DOWN && maze[c][r].down == false)
    can = false;
  if (direction == LEFT && maze[c][r].left == false)
    can = false;
  if (direction == RIGHT && maze[c][r].right == false)
    can = false;
  printf ("heroi pode se movimentar para %d = %d\n", direction, can);
  return can;
};

int whereIsHeroC (void) {
  int i,j,c = 0;
  for (i = 0; i < row; i++)
    for (j = 0; j < column; j++)
      if (maze[i][j].hero == true)
        c = i;
  printf("o heroi está na C %d\n", c);
  return c;
}

int whereIsHeroR (void) {
  int i,j,r = 0;
  for (i = 0; i < row; i++)
    for (j = 0; j < column; j++)
      if (maze[i][j].hero == true)
        r = j;
  printf("o heroi está na R %d\n", r);
  return r;
}

void keyboard(unsigned char key, int x, int y){

  if (screen == GAMEOVER_SCREEN || screen == VICTORY_SCREEN) {
    switch (key) {
    case 27:                                         // tecla Esc (encerra o programa)
      destroyMaze ();
    	exit(0);
      break;
    }
  }

   int c, r;
   c = whereIsHeroC ();
   r = whereIsHeroR ();
  if (screen == GAME_SCREEN) {
    switch (key) {
    case 27:                                         // tecla Esc (encerra o programa)
      destroyMaze ();
    	exit(0);
      break;
    case 'w':
      if (canHeroGo(UP)) {
        if (r < row) {
          maze[c][r].footprint = true;
          maze[c][r].hero = false;
          maze[c][r+1].hero = true;
          steps++;
        }
      }
      break;
    case 's':
      if (canHeroGo(DOWN)) {
        if (r >= 0) {
          maze[c][r].footprint = true;
          maze[c][r].hero = false;
          maze[c][r-1].hero = true;
          steps++;
        }
      }
      break;
    case 'a':
      if (canHeroGo(LEFT)) {
        if (c >= 0) {
          maze[c][r].footprint = true;
          maze[c][r].hero = false;
          maze[c-1][r].hero = true;
          steps++;
        }
      }
      break;
    case 'd':
      if (canHeroGo(RIGHT)) {
        if (c < column) {
          maze[c][r].footprint = true;
          maze[c][r].hero = false;
          maze[c+1][r].hero = true;
          steps++;
        }
      }
      break;
    break;
    }
  
    c = whereIsHeroC ();
    r = whereIsHeroR ();
    if (maze[c][r].end == true) {
      screen = VICTORY_SCREEN;
      printf ("Fim de jogo com %d passos\n", steps);
    }
  }

  if (screen == TITLE_SCREEN) {
    switch (key) {
    case 27:                                         // tecla Esc (encerra o programa)
      destroyMaze ();
    	exit(0);
      break;
    case '1':
      timeLeft = 100;
      break;
    case '2':
      timeLeft = 30;
      break;
    case '3':
      timeLeft = 20;
      break;
    case '4':
      timeLeft = 10;
      break;
    }
    screen = GAME_SCREEN;
    glutTimerFunc(1000,timeCount,1);
  }

  glutPostRedisplay();
}

void createMaze () {
  maze = (struct place **) malloc (row * sizeof(struct place *));
  if (!maze)
    fprintf (stderr, "I could not create a maze\n");
  int i,j;
  for (i = 0; i < row; i++) {
    *(maze + i) = (struct place *) malloc (column * sizeof(struct place));
    if (!(*(maze + i))) {
        fprintf (stderr, "I could not create a maze\n");
        for (j = 0; j < i; j++)
          free (*(maze+i));
        free (maze);
        exit (1);
      }
    }
  for (i = 0; i < row; i++)
    for (j = 0; j < column; j++){
      maze[i][j].up = true;
      maze[i][j].down = true;
      maze[i][j].left = true;
      maze[i][j].right = true;
      if (startC == i && startR == j)
        maze[i][j].start = true;        
      else
        maze[i][j].start = false;        
      if (endC == i && endR == j)
        maze[i][j].end = true;        
      else
        maze[i][j].end = false;        
      if (heroC == i && heroR == j)
        maze[i][j].hero = true;        
      else
        maze[i][j].hero = false;     
      maze[i][j].footprint = false;   
    }

  /* Criar as bordas */
  for (j = 0; j < row; j++) {
    maze[0][j].left = false;
    maze[column-1][j].right = false;
  }
  for (i = 0; i < column; i++) {
    maze[i][0].down = false;
    maze[i][row-1].up = false;
  }

  mazeGenerator ();
  doAnWay ();
  printf ("Labirinto criado\n");
}

void mazeGenerator () {
  srand (seed);
  /* Vou gerar de forma aleatória o labirinto, sem garantia que há solução */
  int i,j;
  int p = mazeProbability;; //probabilidade de 1/p de uma parede ser construída
  for (i = 0; i < column; i++)
    for (j = 0; j < row; j++) {
      if (rand() % p == 0) {
        maze[i][j].up = false;
        if (j < (row - 2))  //essa parte eu uso para garantir que se existe uma parede do canto X que impede ir para X+1, deve existir uma parede no canto X+1 que impede ir para o canto X.
          maze[i][j+1].down = false;
      }
      if (rand() % p == 0) {
        maze[i][j].down = false;
        if (j > 0)
          maze[i][j-1].up = false;
      }
      if (rand() % p == 0) {
        maze[i][j].left = false;
        if (i > 0)
          maze[i-1][j].right = false;
      }
      if (rand() % p == 0) {
        maze[i][j].right = false;
        if (i < (column - 2)) {
          maze[i+1][j].left = false;
        }
      }
    }

    /* Now I will look for close place and make a exit */
    for (i = 0; i < column; i++)
      for (j = 0; j < row; j++) {
        if (maze[i][j].up == false && maze[i][j].down == false && maze[i][j].left == false && maze[i][j].right == false) {
          int open = (rand () % 4) + 1;
          switch (open) {
            case UP:
              if (j < (column - 2)) {
                maze[i][j].up = true;
                maze[i][j+1].down = true;
              }
              break;
            case DOWN:
              if (j > 0) {
                maze[i][j].down = true;
                maze[i][j-1].up = true;
              }
              break;
            case LEFT:
              if (i > 0) {
                maze[i][j].left = true;
                maze[i-1][j].right = true;
              }
              break;
            case RIGHT:
              if (i < (row - 2)) {
                maze[i][j].right = true;
                maze[i+1][j].left = true;
              }
              break;
          }
        }        
      }
}

void destroyMaze () {
    int i;
    for (i = 0; i < row; i++) {
      *(maze + i) = (struct place *) malloc (column * sizeof(struct place));
      if (!(*(maze + i))) {
          fprintf (stderr, "I could not create a maze\n");
          int j;
          for (j = 0; j < i; j++)
            free (*(maze+i));
          free (maze);
      }
    }  
    printf ("Labirinto apagado\n");
    printf ("Fim de jogo com %d passos\n", steps);
}

void output(int x, int y, float r, float g, float b, char *string)
{
  /* Valid font of GLUT
    GLUT_BITMAP_8_BY_13
    GLUT_BITMAP_9_BY_15
    GLUT_BITMAP_TIMES_ROMAN_10
    GLUT_BITMAP_TIMES_ROMAN_24
    GLUT_BITMAP_HELVETICA_10
    GLUT_BITMAP_HELVETICA_12
    GLUT_BITMAP_HELVETICA_18
  */

  glColor3f( r, g, b );
  glRasterPos2f(x, y);
  int len, i;
  len = (int)strlen(string);
  for (i = 0; i < len; i++) {
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string[i]);
  }
}

/* it is a integer to string function I copied and pasted from internet */
char* itoa(int i, char b[]){
    char const digit[] = "0123456789";
    char* p = b;
    if(i<0){
        *p++ = '-';
        i *= -1;
    }
    int shifter = i;
    do{ //Move to where representation ends
        ++p;
        shifter = shifter/10;
    }while(shifter);
    *p = '\0';
    do{ //Move back, inserting digits as u go
        *--p = digit[i%10];
        i = i/10;
    }while(i);
    return b;
}

/* Guaranteeing an exit */ 
void doAnWay (void) {
   srand (seed);
   // Do something ....
   int c, r;
   c = whereIsHeroC ();
   r = whereIsHeroR ();
   //Someday I will do this function
  int pGoUp = 25;
  int pGoDown = 25;
  int pGoRight = 25;
  int pGoLeft = 5;
  int pionnerR = startR; /* "bandeirante" initial position */
  int pionnerC = startC;

  //I put a code to count how many time a direction is chosen and limite consecutive choices
  int countUp = 0;
  int countDown = 0;
  int countLeft = 0;
  int countRight = 0;
  int blockUp = 5;
  int blockDown = 5;
  int blockLeft = 3;
  int blockRight = 4;
  bool risingUp = true;
  bool risingDown = true;
  bool risingLeft = true;
  bool risingRight = true;

  int direction;
  do {
    int prob = rand () % (pGoUp + pGoDown + pGoRight + pGoLeft);
    if (prob >= 0 && prob < pGoUp) {
      if (risingUp) {
        direction = UP;
        countUp++;
        if (countUp == blockUp)
          risingUp = false;
      } else {
        do {
          prob = rand () % (pGoUp + pGoDown + pGoRight + pGoLeft);
        } while (prob == UP);
        countUp--;
        if (countUp == 0)
          risingUp = true;
      }
    }
    if (prob >= pGoUp && prob < (pGoUp + pGoDown)) {
      if (risingDown) {
        direction = DOWN;
        countDown++;
        if (countDown == blockDown)
          risingDown = false;
      } else {
        do {
          prob = rand () % (pGoUp + pGoDown + pGoRight + pGoLeft);
        } while (prob == DOWN);
        countDown--;
        if (countDown == 0)
          risingDown = true;
      }
    }
    if (prob >= (pGoUp + pGoDown) && prob < (pGoUp + pGoDown + pGoRight)) {
      if (risingRight) {
        direction = RIGHT;
        countRight++;
        if (countRight == blockRight)
          risingRight = false;
      } else {
        do {
          prob = rand () % (pGoUp + pGoDown + pGoRight + pGoLeft);
        } while (prob == RIGHT);
        countRight--;
        if (countRight == 0)
          risingRight = true;
      }
    }
    if (prob >= (pGoUp + pGoDown + pGoRight) && prob < (pGoUp + pGoDown + pGoRight + pGoLeft)) {
      if (risingLeft) {
        direction = LEFT;
        countLeft++;
        if (countLeft == blockLeft)
          risingLeft = false;
      } else {
        do {
          prob = rand () % (pGoUp + pGoDown + pGoRight + pGoLeft);
        } while (prob == LEFT);
        countLeft--;
        if (countLeft == 0)
          risingLeft = true;
      }
    }
    //After a direction was chosen open a way
    if (direction == UP && pionnerR < row - 1) {
      maze[pionnerC][pionnerR].up = true;
      pionnerR++;
      if (pionnerR <= row - 1)
        maze[pionnerC][pionnerR].down = true;
    }
    if (direction == DOWN && pionnerR > 0) {
      maze[pionnerC][pionnerR].down = true;
      pionnerR--;
      if (pionnerR > 0)
        maze[pionnerC][pionnerR].up = true;
    }
    if (direction == RIGHT && pionnerC < column - 1) {
      maze[pionnerC][pionnerR].right = true;
      pionnerC++;
      if (pionnerC <= column - 1)
        maze[pionnerC][pionnerR].left = true;
    }
    if (direction == LEFT && pionnerC > 0) {
      maze[pionnerC][pionnerR].left = true;
      pionnerC--;
      if (pionnerC > 0)
        maze[pionnerC][pionnerR].right = true;
    }
    printf ("bandeirante em %d %d\n", pionnerC, pionnerR);
  } while (pionnerR != endR || pionnerC != endC);
}

void timeCount (int value) {
  /* I do not use value, but it mandatory to glutTimerFunc*/
  if (screen == GAME_SCREEN) {
    timeLeft--;
    if (timeLeft == 0)
      screen = GAMEOVER_SCREEN;
    glutPostRedisplay();
    glutTimerFunc(1000,timeCount,1);
  }
}
