#include "kretanja.hpp"
    
MatrixXd Euler2A(double alfa, double beta, double gamma){


	//na osnovu zadatih uglova f-ja vraca matricu A
	

	std::cout << std::endl << "---------------------- \033[4;32mEuler2A\033[0m ------------------------" << std::endl << std::endl;
	std::cout << "\033[32m";

	double a_sin = sin(alfa);
	double a_cos = cos(alfa);

	double b_sin = sin(beta);
	double b_cos = cos(beta);

	double g_sin = sin(gamma);
	double g_cos = cos(gamma);

	MatrixXd RXa;
	MatrixXd RYb;
	MatrixXd RZg;

    RXa.resize(3,3);
    RYb.resize(3,3);
    RZg.resize(3,3);


	RXa <<   1,     0,      0, 
	         0, a_cos, -a_sin,
	         0, a_sin,  a_cos;

	RYb << b_cos, 0, b_sin,
            0,    1,     0,
	        -b_sin, 0, b_cos;

	RZg <<  g_cos, -g_sin, 0,
            g_sin,  g_cos, 0,
            0,         0, 1;


    MatrixXd E2A; 
    E2A.resize(3,3);


	E2A = RZg * RYb * RXa;

	std::cout << "Euler2A:" << std::endl << std::endl;
	std::cout << E2A << std::endl << std::endl;
    //std::cout << "----------------------------------------------------------" << std::endl;
	
	std::cout << "\033[0m";
    return E2A;
}

std::pair<Vector3d, double> AxisAngle(Matrix3d A){

	//f-ja racuna vektor pravca prave i ugao za koji rotiramo na osnovu prosledjene matrice A

	//std::cout  << "---------------------- \033[4;34mAxisAngle\033[0m ---------------------- " << std::endl<< std::endl;
	std::cout << "\033[34m";

    Vector3d p;
    double angle;


	
	Matrix3d E;

	E << 1, 0, 0,
	     0, 1, 0,
	     0, 0, 1;

	Matrix3d ET;

	ET = A.transpose() * A;

	Matrix3d ET_2;
	ET_2 << abs(round(ET(0, 0))), abs(round(ET(0, 1))), abs(round(ET(0, 2))),
	        abs(round(ET(1, 0))), abs(round(ET(1, 1))), abs(round(ET(1, 2))),
	        abs(round(ET(2, 0))), abs(round(ET(2, 1))), abs(round(ET(2, 2)));

	
	//proveravamo unetu matricu: 
	//ako su ispunjeni uslov1 i uslov2 matrica je korektno zadata
	//u suprotnom prijavljujemo gresku i izalzimo iz programa

	//uslov1: At*A = E
	if(ET_2 != E){
		std::cout << "Uneta je losa matrica!" << std::endl<< std::endl<< std::endl;
		return std::make_pair<Vector3d, double>(Vector3d(), -1);
		//exit(EXIT_FAILURE);
	}


	//BUG: uslov !=1 nije dobar
	//uslov2: detA = 1
	if(round(A.determinant()) != 1){
		std::cout << "Uneta je losa matrica!" << std::endl<< std::endl<< std::endl;
		return std::make_pair<Vector3d, double>(Vector3d(), -1);
		//exit(EXIT_FAILURE);
	}

	
	//dva vektora iz (A-E) su normalna na vektor p, odakle sledi da p mozemo da dobijemo
	//kao njihov vektorski proizvod, odabracemo ona dva koja nisu linearno zavisna

	
	//prvi vektor (njegovu sliku trazimo u nastavku) (vektor "u" sa primera i njegova slika "u'")
	Vector3d v1;

	//drugi vektor (koristimo ga samo za )
	Vector3d v2;


	
	//proveravamo linearnu zavisnost vektora, uzimamo prva dva koja nisu linearno zavisna

	Vector3d p1;
	p1 << A(0, 0)-1, A(0, 1), A(0, 2);
	
	Vector3d p2;
	p2 << A(1, 0), A(1, 1)-1, A(1, 2);

	Vector3d p3;
	p3 << A(2, 0), A(2, 1), A(2, 2)-1;


	p1 = p1.normalized();
	p2 = p2.normalized();
	p3 = p3.normalized();

	Vector3d nula;
	nula << 0, 0, 0;

	Vector3d V_Proizvod_1, V_Proizvod_2, V_Proizvod_3;

	V_Proizvod_1 = p1.cross(p2);
	V_Proizvod_1(0) = round(V_Proizvod_1(0));
	V_Proizvod_1(1) = round(V_Proizvod_1(1));
	V_Proizvod_1(2) = round(V_Proizvod_1(2));

	
	V_Proizvod_2 = p1.cross(p3);
	V_Proizvod_2(0) = round(V_Proizvod_2(0));
	V_Proizvod_2(1) = round(V_Proizvod_2(1));
	V_Proizvod_2(2) = round(V_Proizvod_2(2));



	V_Proizvod_3 = p2.cross(p3);
	V_Proizvod_3(0) = round(V_Proizvod_3(0));
	V_Proizvod_3(1) = round(V_Proizvod_3(1));
	V_Proizvod_3(2) = round(V_Proizvod_3(2));


	/*
	if( V_Proizvod_1 !=  nula ){

			std::cout << "Usao 1" <<std::endl;

			v1 = p1; 
			v2 = p2;

	}else if( V_Proizvod_2 != nula){
		
		std::cout << "Usao 2" <<std::endl;

		v1 = p1;
		v2 = p3;

	}else if( V_Proizvod_3 != nula){

		std::cout << "Usao 3" <<std::endl;

		v1 = p2;
		v2 = p3;

	}else{

		std::cout << "Greska!" << std::endl;
		exit(EXIT_FAILURE);
	}
	*/

	if( V_Proizvod_1 !=  nula ){

		//std::cout << "Usao 1" <<std::endl;

		v1 << A(0, 0)-1, A(0, 1), A(0, 2); 
		v2 << A(1, 0), A(1, 1)-1, A(1, 2);

	}else if( V_Proizvod_2 != nula){
		
		//std::cout << "Usao 2" <<std::endl;

		v1 << A(0, 0)-1, A(0, 1), A(0, 2);
		v2 << A(2, 0), A(2, 1), A(2, 2)-1;

	}else if( V_Proizvod_3 != nula){

		//std::cout << "Usao 3" <<std::endl;

		v1 << A(1, 0), A(1, 1)-1, A(1, 2);
		v2 << A(2, 0), A(2, 1), A(2, 2)-1;

	}else{

		std::cout << "Greska!" << std::endl;
		exit(EXIT_FAILURE);
	}
	

	
	//normalizujemo ih
	//v1 = v1.normalized();
	//v2 = v2.normalized();



    //racunamo njihov vektorski proizvod
	p = v1.cross(v2);

	

	//normalizujemo p
	p = p.normalized();


	

	//racunanje slike vektora "u" odnosno vektora v1 koji smo odabrali
	Vector3d v1_p;
	v1_p = A * v1;


	//racunamo ugao preko skalarnog proizvoda i intenziteta
	//ugao = arccos [ (v1 o v1_p) / (|v1|*|v1_p|) ]


	double int1 = sqrt( pow(v1(0),2) + pow(v1(1),2) + pow(v1(2),2) );
	double int2 = sqrt( pow(v1_p(0),2) + pow(v1_p(1),2) + pow(v1_p(2),2) );

	angle = acos(v1.dot(v1_p)/(int1*int2));

	
	//racunamo mesoviti proizvod, ako je on manji od nule uzimamo -p, u suprotnom uzimamo p
	//hocemo da baza v1, v1_p, p bude pozitivno orjentisana da bi rotacija bila u pozitivnom smeru
	
	Matrix3d mesoviti_proizvod;

	mesoviti_proizvod << v1, v1_p, p;

	

	if(mesoviti_proizvod.determinant() < 0){
		p = (-1) * p;
	}

	//std::cout << "AxisAngle: " << std::endl << "ugao: " << angle/M_PI*180 << std::endl << "p: " << std::endl << " " << p << std::endl;

    //std::cout << "----------------------------------------------------------" << std::endl;

	std::cout << "\033[0m";


	return std::make_pair(p,angle);
}

Matrix3d Rodrigez(Vector3d& p, double angle){
	
	p = p.normalized();

	std::cout << std::endl << "---------------------- \033[4;93mRodrigez\033[0m -----------------------" << std::endl << std::endl;
	std::cout << "\033[93m";

	//samo primena formule
	//f-ja prima vektor pravca prave i ugao za koji rotiramo oko te prave
	//vraca matricu rotacije

	Matrix3d P;

	P << 0, -p(2, 0),  p(1, 0),
	    p(2, 0), 0, -p(0, 0),
	    -p(1, 0), p(0, 0), 0;

	
	Matrix3d E;

	E << 1, 0, 0,
	     0, 1, 0,
	     0, 0, 1;

	
	Matrix3d Rez;

	Rez = p*p.transpose() + cos(angle)*(E - p*p.transpose()) + sin(angle)*P;

	std::cout << "Rodrigez: " << std::endl << Rez << std::endl;

    //std::cout << "-----------------------------------------------------" << std::endl;
	std::cout << "\033[0m";

	return Rez;
}

void A2Euler(Matrix3d& RodMat){

	std::cout << std::endl << "---------------------- \033[4;36mA2Euler\033[0m ------------------------" << std::endl << std::endl;
	std::cout << "\033[36m";

	//f-ja, za datu ortogonalnu matricu A vraca Ojlerove uglove
	//(NOTE: po algoritmu sa slajdova)

	double psi, teta, fi; 
	
		if (RodMat(2, 0) < 1){  

			if (RodMat(2, 0) > -1){     //jedinstveno resenje
				
                psi  = atan2(RodMat(1, 0), RodMat(0, 0));
				teta = asin(-RodMat(2, 0));
				fi   = atan2(RodMat(2, 1), RodMat(2, 2));
			
            } else {        //nije jedinstveno resenje, slucaj Ox3 = -Oz
				
				std::cout << "Gimbal Lock! Nije jedinstveno resenje!" << std::endl;
                
				psi  = atan2(-RodMat(0, 1), RodMat(1, 1));
				teta = M_PI/2;
				fi   = 0;
			}
			
		} else {    //nije jedinstveno resenje, slucaj Ox3 = Oz
			
			std::cout << "Gimbal Lock! Nije jedinstveno resenje!" << std::endl;

			psi  = atan2(-RodMat(0, 1), RodMat(1, 1));
			teta = -M_PI/2;
			fi   = 0;
		}

	
    std::cout << "fi: " << fi*180/M_PI << std::endl << "teta: " << teta*180/M_PI << std::endl << "psi: " << psi*180/M_PI << std::endl<< std::endl;

    //std::cout << "--------------------------------------------------------" << std::endl;
	std::cout << "\033[0m";
}

Vector4d Kvaternioni(Vector3d p, double angle){

	
	//na osnovu vektora pravca i ugla racunamo kvaternioon

	//sa slajdova:
	//
	//  p = normalize(p) (ovakav vektor i saljemo f-ji)
	//  w = cos(angle/2)
	//  (x,y,z) = sin(angle/2)*(p0, p1, p2)
	//  
	//	q = xi + yj + zk + w


	p = p.normalized();

	//std::cout << std::endl << "---------------------- \033[4;35mKvaternioni\033[0m --------------------" << std::endl << std::endl;
	


	//std::cout << "AxisAngle2Q:" << std::endl;
	
	Vector4d q;

	

	q << p(0, 0)*sin(angle/2), p(1, 0)*sin(angle/2), p(2, 0)*sin(angle/2), cos(angle/2);

	//std::cout << "Kvaternion: " << std::endl << q << std::endl << std::endl;
	

	return q;

}

std::pair<Vector3d, double> Q2AxisAngle(Vector4d q){

	//std::cout << "Q2AxisAngle:" << std::endl;

	double ugaoFi_Pola = acos(q(3));
	double ugaoFi = ugaoFi_Pola*2;
	
	Vector3d vektor_p;
	vektor_p << q(0)/sin(ugaoFi_Pola), q(1)/sin(ugaoFi_Pola), q(2)/sin(ugaoFi_Pola);

	Vector3d pNorm;
	//double pInt;
	//pInt = sqrt( pow(vektor_p(0),2) + pow(vektor_p(1),2) + pow(vektor_p(2),2)  );
	//pNorm << vektor_p(0)/pInt, vektor_p(1)/pInt, vektor_p(2)/pInt;
	pNorm = vektor_p.normalized();
	

	//std::cout << "Vektor p: " << std::endl << " " << pNorm << std::endl << "Ugao: " << ugaoFi << std::endl;
	

	return std::make_pair(pNorm, ugaoFi/M_PI*180); //PREBACITI UGAO U STEPENE

}