#include <iostream>
#include <GLUT/glut.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <sstream>
#include <math.h>
using namespace std;

void init_setup(int width, int height, char *windowName);
void display_func(void);
void animation_func(int val);
void Keyboard(unsigned char key, int x, int y);
void Special(int key, int x, int y);
void Mouse(int button, int state, int x, int y);
void setupGame();

void displayText(float x, float y, int r, int g, int b, const char *string);

int flag1 = 1, flag2= 1, score = 0;
char ch;
void IntToChar(int number, char *buf)
{
    ostringstream msg;
    msg << number;
    strcpy(buf, msg.str().c_str());
}

//snake struct give head location
struct segment {
    int x;
    int y;
};

class Snake {
    
private:
    char direction;
    segment head;
    segment body[900];
    int length;
    
public:
    //direction set/get
    void setDirection(char input) {
        direction = input;
    };
    
    char getDirection() {
        return direction;
    };
    //head set/get/decrement/increment
    void setHeadX(int input) {
        head.x = input;
    };
    
    int getHeadX() {
        return head.x;
    };
    
    void decHeadX() {
        head.x -= 1;
    };
    
    void incHeadX() {
        head.x += 1;
    };
    
    void setHeadY(int input) {
        head.y = input;
    };
    
    int getHeadY() {
        return head.y;
    };
    
    void decHeadY() {
        head.y -= 1;
    };
    
    void incHeadY() {
        head.y += 1;
    };
    void stopHead(){
        head.x = getHeadX();
        head.y = getHeadY();
    };
    //Body set/get
    void setBodyX(int input, int i) {
        body[i].x = input;
    };
    
    int getBodyX(int i) {
        return body[i].x;
    };
    
    void setBodyY(int input, int i) {
        body[i].y = input;
    };
    
    int getBodyY(int i) {
        return body[i].y;
    };
    //length get/set/increment
    void setLength(int input) {
        length = input;
    };
    
    int getLength() {
        return length;
    };
    //states
    void inBounds() {
        if (head.x < 1 || head.x > 43 || head.y < 1 || head.y > 43) {
            setDirection('X');
        }
    }
    
    void eatSnake() {
        for (int i = 1; i < getLength(); i++) {
            if (getBodyX(i) == getHeadX() && getBodyY(i) == getHeadY())
            {
                setDirection('X');
            }
        }
    };
};

class Fruit {
    
private:
    segment fruit1, fruit2;
    
public:
    void setFruit(Snake snake) {
        do {
            destroy();
            fruit1.x = (rand() % 30) + 1;
            fruit1.y = (rand() % 30) + 1;
            flag1 =  (rand()%2)+1;
            score = score+1;
            
            fruit2.x = (rand() % 40) + 1;
            fruit2.y = (rand() % 40) + 1;
            flag2 =  (rand()%2)+1;
        } while (inSnake(snake));
    };
    
    int getFruitX1() {
        return fruit1.x;
    };
    
    int getFruitY1() {
        return fruit1.y;
    };
    
    int getFruitX2() {
        return fruit2.x;
    };
    
    int getFruitY2() {
        return fruit2.y;
    };
    
    void eatFruit(Snake &snake) {
        if (snake.getHeadX() == getFruitX1() && snake.getHeadY() == getFruitY1()) {
            flag1 = 0;
            //flag2 = 0;
            snake.setLength(snake.getLength() + 1);
            setFruit(snake);
        }
        if (snake.getHeadX() == getFruitX2() && snake.getHeadY() == getFruitY2()) {
            //flag1 = 0;
            flag2 = 0;
            snake.setLength(snake.getLength() + 1);
            setFruit(snake);
        }
    };
    
protected:
    bool inSnake(Snake snake) {
        for (int i = 0; i < snake.getLength(); i++) {
            if ((snake.getBodyX(i) == getFruitX1() && snake.getBodyY(i) == getFruitY1())||(snake.getBodyX(i) == getFruitX2() && snake.getBodyY(i) == getFruitY2())) {
                cout << "fruit true" << endl;
                return true;
            }
            else {
                cout << "fruit false" << endl;
                return false;
            }
        }
        return true;
    };
    
    void destroy() {
        fruit1.x = 40;
        fruit1.y = 40;
    };
};


//  define the window position on screen
int window_x;
int window_y;

//  variables representing the window size
int window_width = 706;
int window_height = 706;

//  variable representing the window title
char *window_title = "Snake";

int ANI_MSEC = 120;

//game board, snake
int gameboard[45][45];
Snake snake;
Fruit fruit;
bool game = true;
char* endgame = "SNAKE DIED";
char* restart = "Press R to Restart";
char* sc = "Score: ";

//random number seed
int seed = time(NULL);


//-------------------------------------------------------------------------
//  Program Main method.
//-------------------------------------------------------------------------
int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    
    init_setup(window_width,window_height,window_title);
    
    glutDisplayFunc(display_func);
    glutKeyboardFunc(Keyboard);
    glutSpecialFunc(Special);
    glutMouseFunc(Mouse);
    glutTimerFunc(ANI_MSEC, animation_func, 0);
    
    setupGame();
    
    glutMainLoop();
    
}
void drawCircle(GLfloat x, GLfloat y, GLfloat radius)
{
    int i;
    float PI = 3.14159;
    int triangleAmount = 1000;
    GLfloat twicePi = 2.0f * PI;
    
    glEnable(GL_LINE_SMOOTH);
    glLineWidth(5.0);
    
    glBegin(GL_LINES);
    glColor4f(1.0, 0.0, 0.0, 1.0);
    for(i = 0; i <= triangleAmount; i++)
    {
        glVertex2f( x, y);
        glVertex2f(x + (radius * cos(i * twicePi / triangleAmount)), y + (radius * sin(i * twicePi / triangleAmount)));
    }
    glEnd();
}

void display_func(void)
{
    int x, y;
    glClearColor(1.0, 1.0, 1.0, 1.0);   // background color (white)
    glClear(GL_COLOR_BUFFER_BIT);       // clearing the buffer not to keep the color
    
    for (int i = 0; i < 45; i++) {
        for (int j = 0; j < 45; j++) {
            if (i == 0 || i == 44 || j == 0 || j == 44) {
                glColor3f(0.0f, 0.0f, 0.0f);
                glBegin(GL_QUADS);
                glVertex2i((i * 16) + 8, (j * 16) + 8);
                glVertex2i((i * 16) + 8, (j * 16) - 8);
                glVertex2i((i * 16) - 8, (j * 16) - 8);
                glVertex2i((i * 16) - 8, (j * 16) + 8);
                glEnd();
            }
            else {
                glColor3f(0.3f, 0.3f, 0.3f);
                glBegin(GL_QUADS);
                glVertex2i((i * 16) + 8, (j * 16) + 8);
                glVertex2i((i * 16) + 8, (j * 16) - 8);
                glVertex2i((i * 16) - 8, (j * 16) - 8);
                glVertex2i((i * 16) - 8, (j * 16) + 8);
                glEnd();
            }
        }
    }
    
    if((flag1>=0)&&(flag1<=1))
    {
        glColor3f(1.0f, 0.5f, 0.5f);
        x = (int)((((fruit.getFruitX1() * 16) + 6)+((fruit.getFruitX1() * 16) - 6))/2);
        y = (int)((((fruit.getFruitY1() * 16) + 6)+((fruit.getFruitY1() * 16) - 6))/2);
        drawCircle(x,y,6.0);
    }
    else if((flag1>1)&&(flag1<=2))
    {
        glColor3f(0.5f,0.5f,1.0f);
        glBegin(GL_QUADS);
        glVertex2i((fruit.getFruitX1() * 16) + 6, (fruit.getFruitY1() * 16) + 6);
        glVertex2i((fruit.getFruitX1() * 16) + 6, (fruit.getFruitY1() * 16) - 6);
        glVertex2i((fruit.getFruitX1() * 16) - 6, (fruit.getFruitY1() * 16) - 6);
        glVertex2i((fruit.getFruitX1() * 16) - 6, (fruit.getFruitY1() * 16) + 6);
        glEnd();
    }
    
    if((flag2>=0)&&(flag2<=1))
    {
        glColor3f(1.0f, 0.5f, 0.5f);
        x = (int)((((fruit.getFruitX2() * 16) + 6)+((fruit.getFruitX2() * 16) - 6))/2);
        y = (int)((((fruit.getFruitY2() * 16) + 6)+((fruit.getFruitY2() * 16) - 6))/2);
        drawCircle(x,y,6.0);
    }
    else if((flag2>1)&&(flag2<=2))
    {
        glColor3f(0.5f,0.5f,1.0f);
        glBegin(GL_QUADS);
        glVertex2i((fruit.getFruitX2() * 16) + 6, (fruit.getFruitY2() * 16) + 6);
        glVertex2i((fruit.getFruitX2() * 16) + 6, (fruit.getFruitY2() * 16) - 6);
        glVertex2i((fruit.getFruitX2() * 16) - 6, (fruit.getFruitY2() * 16) - 6);
        glVertex2i((fruit.getFruitX2() * 16) - 6, (fruit.getFruitY2() * 16) + 6);
        glEnd();
    }
    
    glColor3f(0.4f, 1.0f, 0.4f);
    for (int i = 1; i < snake.getLength(); i++) {
        glBegin(GL_QUADS);
        glVertex2i((snake.getBodyX(i) * 16) + 9, (snake.getBodyY(i) * 16) + 9);
        glVertex2i((snake.getBodyX(i) * 16) + 9, (snake.getBodyY(i) * 16) - 9);
        glVertex2i((snake.getBodyX(i) * 16) - 9, (snake.getBodyY(i) * 16) - 9);
        glVertex2i((snake.getBodyX(i) * 16) - 9, (snake.getBodyY(i) * 16) + 9);
        glEnd();
    }
    
    glColor3f(0.0f, 0.5f, 0.7f);
    glBegin(GL_QUADS);
    glVertex2i((snake.getHeadX() * 16) + 9, (snake.getHeadY() * 16) + 9);
    glVertex2i((snake.getHeadX() * 16) + 9, (snake.getHeadY() * 16) - 9);
    glVertex2i((snake.getHeadX() * 16) - 9, (snake.getHeadY() * 16) - 9);
    glVertex2i((snake.getHeadX() * 16) - 9, (snake.getHeadY() * 16) + 9);
    glEnd();
    
    
    if (!game) {
        IntToChar((score-1),&ch);
        displayText(280, 390, 1, 0, 1, endgame);
        displayText(265, 360, 0, 1, 1, restart);
        displayText(305, 330, 1, 1, 0, sc);
        displayText(375, 330, 1, 1, 0, &ch);
    }
    
    glFlush();
    
    glutSwapBuffers();    // double buffering
}    // end of display_func()

void reshape_handler(int width, int height)
{
    glViewport(0, 0, width, height);                            // sets the viewport
    glMatrixMode(GL_PROJECTION);                                // projection matrix
    glLoadIdentity();                                            // loads identity matrix
    gluOrtho2D(0.0, (GLdouble)width, 0.0, (GLdouble)height);    // 2D orthographic projection
}    // end of reshape_handler()

void centerOnScreen()
{
    window_x = (glutGet(GLUT_SCREEN_WIDTH) - window_width) / 2;
    window_y = (glutGet(GLUT_SCREEN_HEIGHT) - window_height) / 2;
}

void init_setup(int width, int height, char *windowName)
{
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);  // single buffer, rgb color
    centerOnScreen();
    glutInitWindowSize(window_width,window_height );              // init. window size
    glutInitWindowPosition(window_x, window_y);                  // init. window position
    glutCreateWindow(windowName);                  // window name
    glutReshapeFunc(reshape_handler);              // sets the reshape call back
}    // end of init_setup()

void animation_func(int val)
{
    switch (snake.getDirection()) {
        case 'L':
            snake.decHeadX();
            break;
        case'U':
            snake.incHeadY();
            break;
        case'R':
            snake.incHeadX();
            break;
        case'D':
            snake.decHeadY();
            break;
        case'X':
            snake.stopHead();
            game = false;
            break;
            
        default:
            break;
    }
    if (game){
        for (int i = snake.getLength(); i >= 0; i--) {
            if (i == 0) {
                snake.setBodyX(snake.getHeadX(), i);
                snake.setBodyY(snake.getHeadY(), i);
            }
            else {
                snake.setBodyX(snake.getBodyX(i - 1), i);
                snake.setBodyY(snake.getBodyY(i - 1), i);
            }
        }
    }
    
    fruit.eatFruit(snake);
    
    snake.inBounds();
    snake.eatSnake();
    
    glutPostRedisplay();
    
    glutTimerFunc(ANI_MSEC, animation_func, 0);
    
}//end animation_func

void Keyboard(unsigned char key, int x, int y)
{
    if (snake.getDirection() == 'X') {
        if (key == 'r') {//reset game
            score = 0;
            setupGame();
        }
    }
    else {
        switch (key) {
            case 'w':
                if(snake.getDirection() != 'D')
                    snake.setDirection('U');
                break;
            case 's':
                if (snake.getDirection() != 'U')
                    snake.setDirection('D');
                break;
            case 'd':
                if (snake.getDirection() != 'L')
                    snake.setDirection('R');
                break;
            case 'a':
                if (snake.getDirection() != 'R')
                    snake.setDirection('L');
                break;
                
            default:
                break;
        }
    }
    glutPostRedisplay();
}

void Special(int key, int x, int y) {
    
    if (snake.getDirection() != 'X') {
        switch (key) {
            case GLUT_KEY_UP:
                if (snake.getDirection() != 'D')
                    snake.setDirection('U');
                break;
            case GLUT_KEY_DOWN:
                if (snake.getDirection() != 'U')
                    snake.setDirection('D');
                break;
            case GLUT_KEY_RIGHT:
                if (snake.getDirection() != 'L')
                    snake.setDirection('R');
                break;
            case GLUT_KEY_LEFT:
                if (snake.getDirection() != 'R')
                    snake.setDirection('L');
                break;
                
            default:
                break;
        }
    }
    glutPostRedisplay();
}

void Mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        
    }
    
    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        
    }
}

void setupGame() { //initialize gameboard and snake
    //set seed
    srand(seed);
    
    for (int i = 0; i < 44; i++) {
        for (int j = 0; j < 44; j++) {
            if (i == 0 || i == 43 || j == 0 || j == 43)
                gameboard[j][i] = 2;
            else
                gameboard[j][i] = 0;
        }
    }
    
    game = true;
    snake.setDirection('R');
    snake.setLength(5);
    
    for (int i = 0; i < 900; i++) {
        if (i <= 2) {
            snake.setBodyX(16 - i, i);
            snake.setBodyY(16, i);
        }
        else {
            snake.setBodyX(-1, i);
            snake.setBodyY(-1, i);
        }
    }
    
    snake.setHeadX(16);
    snake.setHeadY(16);
    
    fruit.setFruit(snake);
}

//Copy Paste of gl text example
void displayText(float x, float y, int r, int g, int b, const char *string) {
    int j = strlen(string);
    
    glColor3f(r, g, b);
    glRasterPos2f(x, y);
    for (int i = 0; i < j; i++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
    }
}

