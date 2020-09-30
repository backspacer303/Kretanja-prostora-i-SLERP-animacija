#define TIMER0 0
#define TIMER1 1

#include <iostream>
#include <cstdlib>
#include <cmath>
#include <utility>

#include <GL/glut.h>
#include <GL/glu.h>

#include "eigen/Eigen/Eigen"
#include "kretanja.hpp"

using namespace Eigen;


static void on_keyboard(unsigned char key,int x, int y);
static void on_display(void);
static void on_reshape(int width, int height);
static void on_timer(int value);
void draw_axes();
void set_lights();
void set_materials(std::string material_name);
Matrix4d save_V_inveerse();
Matrix4d getM(Matrix4d Vinv);
Vector3d LinearnaInterpolacija(Vector3d c1, Vector3d c2, double t);
Vector4d SLERP(Vector4d q1, Vector4d q2, double t);


//****GLOBALNI PARAMETRI ANIMACIJE****//

int animation_ongoing = 0;
static int window_width, window_height;

double t = 0;
double tu = 10.0;

//globalni nizovi za materijale
GLfloat ambient_coeffs[4];
GLfloat diffuse_coeffs[4];
GLfloat specular_coeffs[4];
GLfloat shininess;



int main(int argc, char *argv[]){

    glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

	//pravmo prozor
	glutInitWindowPosition(100,100);
	glutInitWindowSize(1000,800);
	glutCreateWindow("SLERP");

	//pozivi callback funkcija
	glutKeyboardFunc(on_keyboard);
	glutDisplayFunc(on_display);
	glutReshapeFunc(on_reshape);

	//boja za ciscenje prozora
	glClearColor(0, 0, 0, 0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
    
	
    //glavna petlja
	glutMainLoop();


    return 0;
}

static void on_keyboard(unsigned char key,int x, int y){

	switch(key){
		
        //izlazak iz programa na 'Esc'
		case 27:
			exit(EXIT_SUCCESS);

		//pokretanje animacije        
		case 'b':
			if(!animation_ongoing){
				animation_ongoing = 1;
				glutTimerFunc(50, on_timer, TIMER0);
			}
			break;
        

		//zaustavljanje animacije        
		case 's':
			if(animation_ongoing){
				animation_ongoing = 0;
				glutTimerFunc(50, on_timer, TIMER0);
			}
			break;  
		

		
	}
}


static void on_timer(int value){
	
        
	t += 0.05;


	//u ovom trenutku zaustavljamo animaciju i vracamo parametre na pocetne vrednosti kako
	//bismo mogli ponovnim pritiskom tastera 'b' da opet pokrenemo animaciju ispocetka
	if(t >= tu){  

		t = 0;
		animation_ongoing = 0; 
	}
    



    /* Forsira se ponovno iscrtavanje prozora. */
    glutPostRedisplay();

    /* Po potrebi se ponovo postavlja tajmer. */
    if (animation_ongoing)
   		glutTimerFunc(1, on_timer, TIMER0);
    	
}

static void on_reshape(int width, int height){
	window_width = width;
	window_height = height;
}


static void on_display(void){



	//cistimo buffere
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 	
    //gasimo svetlo (da ne bi ometalo iscrtavanje koord. osa)
    glDisable(GL_LIGHTING);

	glViewport(0, 0, window_width, window_height);

	//postaljamo projekciju
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, window_width/(float)window_height, 1, 1500);
	

    //ucitavamo jedinicnu MODELVIEW matricu
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
    //zadajemo kameru
	gluLookAt(6 ,        //x 
			  6 ,        //y 
			  6 ,        //z 
			  0, 0, 0,	 //loook-at
			  0, 1, 0);  //up

	
	
	auto Vinv = save_V_inveerse();
	//auto M = getM(Vinv); 
	//std::cout << "M:" << std::endl << M << std::endl;

    
	
	//pozivamo f-ju za iscrtavanje koordinatnih osa i postavljanje osvetljenja
	draw_axes();
	set_lights();
    



    //Ovde ide crtanje objekata...
    set_materials(std::string("tirquoise"));
	

	Matrix3d A;
	Matrix4d M;
	std::pair<Vector3d, double> rez;
	
	Vector3d c1;
	Vector3d c2;
	Vector3d ct;

	Vector4d q1;
	Vector4d q2;
	Vector4d qt;

	//POCETNI OBJEKAT
	glPushMatrix();
	
		glRotatef(-45, 1, 0, 0);
		glTranslatef(0,0,4);	
		glutSolidTorus(0.3,0.4,100,100);

		glDisable(GL_LIGHTING);
		glBegin(GL_LINES);
			glColor3f(1, 0, 0);
			glVertex3f(0, 0, 0);
			glVertex3f(1.2, 0, 0);

			glColor3f(0, 1, 0);
			glVertex3f(0, 0, 0);
			glVertex3f(0, 1.2, 0);

			glColor3f(0, 0, 1);
			glVertex3f(0, 0, 0);
			glVertex3f(0, 0, 1.2);
    	glEnd();
		glEnable(GL_LIGHTING);


		M = getM(Vinv);

		c1 << M(0,3), M(1,3), M(2,3);		

		//std::cout << "Centar1:" << round(c1(0)) << " " << round(c1(1)) << " " << round(c1(2)) << std::endl;

		A << M(0,0), M(0,1), M(0,2),
			 M(1,0), M(1,1), M(1,2),
			 M(2,0), M(2,1), M(2,2);
		
		//std::cout << "Polozaj1:" << std::endl;
		//A2Euler(A);


		rez = AxisAngle(A);		

		q1 = Kvaternioni(rez.first, rez.second);

	glPopMatrix();





	//KRAJNJI OBJEKAT	
	glPushMatrix();

		glRotatef(70, 0, 1, 0);	
		glTranslatef(0,0,4);
		glutSolidTorus(0.3,0.4,100,100);		
		
		glDisable(GL_LIGHTING);
		glBegin(GL_LINES);
			glColor3f(1, 0, 0);
			glVertex3f(0, 0, 0);
			glVertex3f(1.2, 0, 0);

			glColor3f(0, 1, 0);
			glVertex3f(0, 0, 0);
			glVertex3f(0, 1.2, 0);

			glColor3f(0, 0, 1);
			glVertex3f(0, 0, 0);
			glVertex3f(0, 0, 1.2);
    	glEnd();
		glEnable(GL_LIGHTING);


		M = getM(Vinv);

		c2 << M(0,3), M(1,3), M(2,3);
		
		//std::cout << "Centar2:" << round(c2(0)) << " " << round(c2(1)) << " " << round(c2(2)) << std::endl;

		A << M(0,0), M(0,1), M(0,2),
			 M(1,0), M(1,1), M(1,2),
			 M(2,0), M(2,1), M(2,2);
		
		//std::cout << "Polozaj2:" << std::endl;
		//A2Euler(A);
		
		
		rez = AxisAngle(A);

		q2 = Kvaternioni(rez.first, rez.second);

	glPopMatrix();
	

	ct = LinearnaInterpolacija(c1, c2, t);
	qt = SLERP(q1, q2, t);


	auto qtAxiAngle = Q2AxisAngle(qt);

	//ISCRTAVANJE NOVOG OBJEKTA
	glPushMatrix();

		glTranslatef(ct(0), ct(1), ct(2));
		glRotatef(qtAxiAngle.second,
		          qtAxiAngle.first(0), 
				  qtAxiAngle.first(1), 
				  qtAxiAngle.first(2)
				);			
		glutSolidTorus(0.3,0.4,100,100);

		glDisable(GL_LIGHTING);
		glBegin(GL_LINES);
			glColor3f(1, 0, 0);
			glVertex3f(0, 0, 0);
			glVertex3f(1.2, 0, 0);

			glColor3f(0, 1, 0);
			glVertex3f(0, 0, 0);
			glVertex3f(0, 1.2, 0);

			glColor3f(0, 0, 1);
			glVertex3f(0, 0, 0);
			glVertex3f(0, 0, 1.2);
    	glEnd();
		glEnable(GL_LIGHTING);


	glPopMatrix();

	

	//zamenjujemo sadrzaje buffera
	glutSwapBuffers();
}

//------------------------------------------------------------
void draw_axes(){

	glPushMatrix();

    glBegin(GL_LINES);
        glColor3f(1, 0, 0);
        glVertex3f(0, 0, 0);
        glVertex3f(1000, 0, 0);

        glColor3f(0, 1, 0);
        glVertex3f(0, 0, 0);
        glVertex3f(0, 1000, 0);

        glColor3f(0, 0, 1);
        glVertex3f(0, 0, 0);
        glVertex3f(0, 0, 1000);
    glEnd();

    glPopMatrix();
}

void set_lights(){


	glShadeModel(GL_SMOOTH);

	GLfloat light_position[] = { 1, 1, 1, 0 };
    
    GLfloat light_ambient[]  = { 0.3, 0.3, 0.3, 1 }; //ono sto se dobija po sobi od predmete
    GLfloat light_diffuse[]  = { 0.7, 0.7, 0.7, 1 }; //ono sto direktno dolazi iz izvora svetlosti
    GLfloat light_specular[] = { 0.9, 0.9, 0.9, 1 }; //odlsja, beli krug, ima ga metal



	glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    
    


}

void set_materials(std::string material_name){


	if(!material_name.compare("white")){
		ambient_coeffs[0]  = 0.9;   ambient_coeffs[1]  = 0.9;  ambient_coeffs[2]  = 0.9; ambient_coeffs[3]  = 1;
		diffuse_coeffs[0]  = 0.55;  diffuse_coeffs[1]  = 0.55;  diffuse_coeffs[2]  = 0.55; diffuse_coeffs[3]  = 1;
		specular_coeffs[0] = 0.7; specular_coeffs[1] = 0.7; specular_coeffs[2] = 0.7; specular_coeffs[3] = 1;
		shininess = 0.25*128;
	    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
		glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coeffs);
		glMaterialf(GL_FRONT, GL_SHININESS, shininess);
	}

	if(!material_name.compare("gold")){
		ambient_coeffs[0]  = 0.24725;   ambient_coeffs[1]  = 0.1995;  ambient_coeffs[2]  = 0.0745; ambient_coeffs[3]  = 1;
		diffuse_coeffs[0]  = 0.75164;  diffuse_coeffs[1]  = 0.60648;  diffuse_coeffs[2]  = 0.22648; diffuse_coeffs[3]  = 1;
		specular_coeffs[0] = 0.628281; specular_coeffs[1] = 0.555802; specular_coeffs[2] = 0.366065; specular_coeffs[3] = 1;
		shininess = 0.4*128;
	    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
		glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coeffs);
		glMaterialf(GL_FRONT, GL_SHININESS, shininess);
	}

	if(!material_name.compare("ruby_red")){
		ambient_coeffs[0]  = 0.1745;   ambient_coeffs[1]  = 0.01175;  ambient_coeffs[2]  = 0.01175; ambient_coeffs[3]  = 1;
		diffuse_coeffs[0]  = 0.61424;  diffuse_coeffs[1]  = 0.04136;  diffuse_coeffs[2]  = 0.04136; diffuse_coeffs[3]  = 1;
		specular_coeffs[0] = 0.727811; specular_coeffs[1] = 0.626959; specular_coeffs[2] = 0.626959; specular_coeffs[3] = 1;
		shininess = 0.6*128;
	    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
		glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coeffs);
		glMaterialf(GL_FRONT, GL_SHININESS, shininess);	
	}

	if(!material_name.compare("green")){
		ambient_coeffs[0]  = 0.135;    ambient_coeffs[1]  = 0.2225;   ambient_coeffs[2]  = 0.1575; ambient_coeffs[3]  = 1;
		diffuse_coeffs[0]  = 0.54;     diffuse_coeffs[1]  = 0.89;     diffuse_coeffs[2]  = 0.63; diffuse_coeffs[3]  = 1;
		specular_coeffs[0] = 0.316228; specular_coeffs[1] = 0.316228; specular_coeffs[2] = 0.316228; specular_coeffs[3] = 1;
		shininess = 0.1*128;
		glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
		glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coeffs);
		glMaterialf(GL_FRONT, GL_SHININESS, shininess);
	}

	if(!material_name.compare("blue")){
		ambient_coeffs[0]  = 0.4; ambient_coeffs[1]  = 0.4; ambient_coeffs[2]  = 0.9; ambient_coeffs[3]  = 1;
		diffuse_coeffs[0]  = 0.4; diffuse_coeffs[1]  = 0.4; diffuse_coeffs[2]  = 0.9; diffuse_coeffs[3]  = 1;
		specular_coeffs[0] = 0.5; specular_coeffs[1] = 0.5; specular_coeffs[2] = 0.5; specular_coeffs[3] = 1;
		shininess = 0.1*100;
		glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
		glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coeffs);
		glMaterialf(GL_FRONT, GL_SHININESS, shininess);	
	}

	

	if(!material_name.compare("tirquoise")){
		ambient_coeffs[0]  = 0.1; ambient_coeffs[1]  = 0.18725; ambient_coeffs[2]  = 0.1745; ambient_coeffs[3]  = 1;
		diffuse_coeffs[0]  = 0.396; diffuse_coeffs[1]  = 0.74151; diffuse_coeffs[2]  = 0.69102; diffuse_coeffs[3]  = 1;
		specular_coeffs[0] = 0.297254; specular_coeffs[1] = 0.30829; specular_coeffs[2] = 0.306678; specular_coeffs[3] = 1;
		shininess = 0.1*100;
		glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
		glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coeffs);
		glMaterialf(GL_FRONT, GL_SHININESS, shininess);
	}	


	if(!material_name.compare("light_blue")){
		ambient_coeffs[0]  = 0.4; ambient_coeffs[1]  = 0.4; ambient_coeffs[2]  = 0.9; ambient_coeffs[3]  = 1;
		diffuse_coeffs[0]  = 0.4; diffuse_coeffs[1]  = 0.4; diffuse_coeffs[2]  = 0.9; diffuse_coeffs[3]  = 1;
		specular_coeffs[0] = 0.5; specular_coeffs[1] = 0.5; specular_coeffs[2] = 0.5; specular_coeffs[3] = 1;
		shininess = 100;
		glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
		glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coeffs);
		glMaterialf(GL_FRONT, GL_SHININESS, shininess);	
	}



}
//------------------------------------------------------------
Matrix4d save_V_inveerse(){

	double mv[16];
	glGetDoublev(GL_MODELVIEW_MATRIX, mv);

	Matrix4d MVraw;
	MVraw << mv[0],  mv[1],  mv[2],  mv[3],
		     mv[4],  mv[5],  mv[6],  mv[7],
		     mv[8],  mv[9],  mv[10], mv[11],
		     mv[12], mv[13], mv[14], mv[15];

	//u tom trenutku imamo samo matricu V u MODELVIEW matrici
	Matrix4d V;
	V = MVraw.transpose();

	Matrix4d Vinv;
	Vinv = V.inverse();

	return Vinv;
}

Matrix4d getM(Matrix4d Vinv){

	double mv[16];
	glGetDoublev(GL_MODELVIEW_MATRIX, mv);

	Matrix4d MVraw;
	MVraw << mv[0],  mv[1],  mv[2],  mv[3],
		     mv[4],  mv[5],  mv[6],  mv[7],
		     mv[8],  mv[9],  mv[10], mv[11],
		     mv[12], mv[13], mv[14], mv[15];

	Matrix4d MV; 
	MV = MVraw.transpose();

	
	Matrix4d M;
	M = Vinv * MV;

	return M;
}
//------------------------------------------------------------
Vector3d LinearnaInterpolacija(Vector3d c1, Vector3d c2, double t){

	Vector3d ct;

	ct = (1 - t/tu)*c1 + (t/tu)*c2;

	return ct;
}

Vector4d SLERP(Vector4d q1, Vector4d q2, double t){

	double cosO;
	cosO = q1.dot(q2);

	if(cosO < 0){ 		//idi po kracem luku sfere
		q1 = (-1)*q1;
		cosO = (-1)*cosO;
	}

	if(cosO > 0.95){
		return q1;
	}

	double fi;
	fi = acos(cosO);

	Vector4d qt;

	qt = (sin(fi*(1-t/tu))/sin(fi))*q1 + (sin(fi*(t/tu))/sin(fi))*q2;

	return qt;
}
