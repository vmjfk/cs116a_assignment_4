
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>


#define MAIN_WINDOW_WIDTH 640
#define MAIN_WINDOW_HEIGHT 480


void loadShapes()
{

}

void reshow()
{

    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
    printf("button = %d, state = %d, x = %d, y = %d\n",button,state,x,y);
 
}
void mainMenu(int value)
{  
}

void init(void)
{

    glClearColor(0.0,0.0,0.0,0.0);
 


}

void display()
{
    printf("display called\n");
    glClear(GL_COLOR_BUFFER_BIT);

    loadShapes();
    glFlush();
    glutSwapBuffers();

}


void reshape(int width, int height)
{
    printf("reshape called.\n");
    glViewport(0,0,width,height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    GLfloat aspect = (GLfloat) width/(GLfloat)height;
    gluPerspective(45.0f,aspect, 0.1f,100.0f);  
    glMatrixMode(GL_MODELVIEW);
}
void buildMenus()
{
    printf("buildMenus called.\n");

 
    char* color[8];
    color[0] = "red";
    color[1] = "green";
    color[2] = "blue";
    color[3] = "yellow";
    color[4] = "purple";
    color[5] = "orange";
    color[6] = "white";
    color[7] = "black";

    
    int ctr = 0;
    int rectFilledColorMenu = glutCreateMenu(mainMenu);
    for(int i = 0;i<8;i++)
    {
        glutAddMenuEntry(color[i],i + ctr*10);
    }

    ctr++;
    int rectOutlineColorMenu = glutCreateMenu(mainMenu);
    for(int i = 0;i<8;i++)
    {
        glutAddMenuEntry(color[i],i + ctr*10);
    }

    ctr++;
    int ellipseFilledColorMenu = glutCreateMenu(mainMenu);
    for(int i = 0;i<8;i++)
    {
        glutAddMenuEntry(color[i],i + ctr*10);
    }
    
    ctr++;
    int ellipseOutlineColorMenu = glutCreateMenu(mainMenu);
    for(int i = 0;i<8;i++)
    {
        glutAddMenuEntry(color[i],i + ctr*10);
    }

    ctr++;
    int lineMenu = glutCreateMenu(mainMenu);
    for(int i = 0;i<8;i++)
    {
        glutAddMenuEntry(color[i],i + ctr*10);
    }

    ctr++;
    int bezMenu = glutCreateMenu(mainMenu);
    for(int i = 0;i<8;i++)
    {
        glutAddMenuEntry(color[i],i + ctr*10);
    }

    int rectMenu = glutCreateMenu(mainMenu);
    glutAddSubMenu("filled",rectFilledColorMenu);
    glutAddSubMenu("outline",rectOutlineColorMenu);

    int ellipseMenu = glutCreateMenu(mainMenu);
    glutAddSubMenu("filled",ellipseFilledColorMenu);
    glutAddSubMenu("outline",ellipseOutlineColorMenu);

 

    int mnMenu = glutCreateMenu(mainMenu);
    glutAddSubMenu("rectangle",rectMenu);
    glutAddSubMenu("ellipse",ellipseMenu);
    glutAddSubMenu("line",lineMenu);
    glutAddSubMenu("bezier",bezMenu);


    
    glutAttachMenu(GLUT_LEFT_BUTTON);
}

int main (int argc, char *argv[])
{  

	glutInit (&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize (MAIN_WINDOW_WIDTH, MAIN_WINDOW_HEIGHT);
	glutInitWindowPosition (0,0);
	glutCreateWindow ("MyPaint");
    
    buildMenus();

    init();	
    glutDisplayFunc (display);
	glutReshapeFunc (reshape);
	glutMouseFunc (mouse);


  
	glutMainLoop ();
}

