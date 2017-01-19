/*----------------------------------------------------------------------------*/
/* ARBOL DE BUSQUEDA BINARIO                                                  */
/*                                                                            */
/* Creadores:   FACEB                                                         */
/*----------------------------------------------------------------------------*/


#include "srgp.h"	//libreria grafica
#include "stdio.h"	//libreria standard
#include "stdlib.h"	//libreria para el manejo de funciones de utileria
#include "time.h"	 //libreria para el manejo de funciones de fecha y hora
#include "string.h" //no se
#include "math.h" //libreria para ocupar pow

#define SH	20
#define SV	30
#define ANCHO 700	//define el ancho de la ventana grafica en pixeles
#define ALTO 670	//define el alto de la ventana grafica en pixeles
#define NCOLORS 13	//define el numero de colores a utilizar
#define NFONTS 11	//define el numero de fuentes a utilizar
#define BT 9		//define el numero de botones maximo
#define B 3		//define el tamano para el buffer

/*Estructura que almacena todos los atributos de un boton dentro del programa*/
typedef struct{
	point pi,pf;	//indica las coordenadas x,y del punto inicial y final
	char *nombre;	//variable donde se almacenara el nombre del boton
}boton;

/*Estructura que almacena las coordenadas iniciales y finales de un circulo*/
typedef struct{
	point pi,pf;	//almacena el punto inicial y final x,y de cada circulo
}circulo;

/*Estructura que almacena todos los atributos de una caja de texto dentro del programa*/
typedef struct{
	point posicion;		//almacena la posicion inicial para colocar el texbox
	int ancho, alto;	//almacena el ancho y alto de el texbox
	char *cadena;		//donde se escribira lo introducido desde el teclado
	int longitud;		//almacena la maxima longitud de la cadena
	int escribir;		//sirve para controlar cuando se escribe y cuando no
}cuadro;

/* Estructura que almacena todos los atributos de los nodos*/
typedef struct Nodo{
	int item;
	struct Nodo *izq,*der;
} Nodo;

/*********************************Declaracion de arreglos y variables globales*/
boton botones[BT];	//Arreglo que almacena toda la informacion de los botones (menos el de la carita)
circulo circulos[3];	//Arreglo que almacena la informacion de los circulos de la clave de ordenacion
cuadro cuadros[4];	//Arreglo que alamacena toda la informacion de los cuadros de texto

char *colorNames[NCOLORS]={	//arreglo que contiene los nombres de los colores a utilizar
	"white","black","red","darkblue","gray","green","yellow","orange","blue",
	"dimgrey","lightgray","SlateGray","ForestGreen"
};
char *fontNames[NFONTS]={	//arreglo que contiene los nombres de las fuentes a utilizar
	"lucidasans-bold-12","lucidasans-bolditalic-14","lucidasans-8","lucidasans-10",
	"lucidasans-12","lucidasans-bold-8","lucidasans-bold-10","lucidasans-bolditalic-8",
	"lucidasans-bolditalic-10","lucidasans-bolditalic-12","lucidasans-bold-14"
};

/******************************************** Declaracion de variables globales *******************************/
int NBotones = 0;	//Variable global entera que llebara el control de la cantidad de botones
int NCirculos = 0;	//Variable global entera que llebara el control de la cantidad de circulos de chequeo
int NCuadros = 0;	//Variable global entera que llebara el control de la cantidad de cuadros de texto
int indice = 0;		//variable global que almacena el indice del cuadro de texto activo
int BAD = FALSE;	//variable que sirve para el boton aceptar dentro del lienzo Acerca de...
char buffer[B];		//sirve para almacenar temporalmente los caracteres o cadenas que se introducen del teclado
char archivoIE[11]; //arreglo temporal para capturar el name del archivo leido
char cat[81];	//arreglo que almacena el contenido actual del cuadro de pares ordenados
int xxx;	//variable entera que almacena la comversion de lo ingresado en la casilla de item
Nodo *T = NULL; //apuntador a la cabexa del arbol

deluxe_locator_measure dlm;	/*variable global para manejar el raton, contiene informacion de la posicion
				  del puntero del raton y del estado de los botones(arriba o abajo)*/
deluxe_keyboard_measure dkm;	/*variable global para manejar el teclado, contiene informacion de los caracteres
				  o cadenas que se introducen desde el teclado*/
canvasID lienzo,lienzoText,lienzoAcerca;	//declara un identificador (ID) para un lienzo

/******************************************* Declaracion de todas las funciones***********************************/
void parametrosIniciales(void);
void dibujarInterfaz(void);
void definirColores(void);
void definirFuentes(void);
void procesarEventos(void);
void procesarRaton(void);
void procesarTeclado(void);
void refrescar(void);
void iniciarBoton(int x,int y,int ancho,int alto,char *nombre);
void dibujarBoton(int index,int p);
int getIndex(int x,int y,int q);
void dibujarEfecto(int xi,int yi,int xf,int yf,int grosorLinea,int p);
void dibujarCirculo(int index,int chequeo);
void iniciarCirculo(int x,int y,int chequeo);
void iniciarCuadro(int x,int y,int ancho,int alto,int longitud);
void dibujarCuadro(int index,int chequeo);
void clearText(void);
void acercaDe(void);
void dibujarArbolB(void);
void clear(void);
void dibujarLogoDEI(int x,int y);
void dibujarLogoUCA(int x,int y);


/******************************* funciones para la utilizacion de archivos *********************************/
void importaArchivo(char *cadena);
void exportaArchivo(char *cadena);

/**************************** funciones para la utilizacion de los nodos ************************************/
void leerCadena(char *cad,int n);
Nodo *crear(void);
void insertar(Nodo **T,int item);
void insertarAux(Nodo **T,Nodo *temp);
int mayorNodo(Nodo *T);
void eliminar(Nodo **T,int item);
void eliminarTodo(Nodo **T);
void mostrar(Nodo *T);
void construye(char *cad,Nodo **T);
void construyeAux(char *cad,Nodo **T,int I,int F);
int determinaFronteras(int *r,int *II,int *FI,int *ID,int *FD,char *cad,int I,int F);
int altura(Nodo *T);
int alturaAux(Nodo *T,int actual,int mayor);
int di(int n);
/****************************** funciones para la creacion de los nodos ************************************/
//void dibujarNodo(int x,int y);
void dibujar(Nodo *T, int x,int y,int colorOvalo,int colorLinea,int colorTexto);



int main(){
	//esta funcion sirve para crear la ventana grafica del programa
	SRGP_begin("DEPARTAMENTO DE ELECTRONICA E INFORMATICA - CICLO 01/2006",ANCHO,ALTO,8,FALSE);
	parametrosIniciales();	//activa todos los parametros
	dibujarInterfaz();	//dibuja la interfaz inicialmente
	procesarEventos();	//administra las entradas de mouse y teclado
	SRGP_end();	//destruye la ventana grafica creada
}

//------------------------------------------------------------------------------
//Funcion que carga todos los parametros de color, fuente, modos del teclado
//y del mouse, para ser ocupados en la interfaz grafica del programa
//------------------------------------------------------------------------------
void parametrosIniciales(){
	//-----------------------------------------------
	definirColores();	//carga los colores en un indice
	definirFuentes();	//carga las fuentes en un indice
	//-----------------------------------------------
	//sirve para iniciar el dispositivo raton, en modo:"EVENT"
	SRGP_setInputMode(LOCATOR,EVENT);
	//-----------------------------------------------
	//inicializa los botones que ocupara el raton (LEFT_BUTTON_MASK|RIGHT_BUTTON_MASK|MIDDLE_BUTTON_MASK)
	SRGP_setLocatorButtonMask(LEFT_BUTTON_MASK);
	//-----------------------------------------------
	//iniciliza en que modo procesara el texto introducido desde el teclado, RAW: lee caracter por caracter
	SRGP_setKeyboardProcessingMode(RAW);
	//-----------------------------------------------
	//sirve para iniciar el dispositivo teclado, en modo: "EVENT"
	SRGP_setInputMode(KEYBOARD,EVENT);
	//-----------------------------------------------
	//inicializa el buffer de la variable global donde se guardaran los caracteres o cadenas
	dkm.buffer = buffer;
	//le asigna la longitud al buffer, el cual fue definido con 2
	dkm.buffer_length = 2;
	//-----------------------------------------------
	/*con el identidicador declarado anteriormente llamado lienzo se crea el lienzo temporal con SRGP_createCanvas*/
	lienzo = SRGP_createCanvas(ANCHO,ALTO);
}

//------------------------------------------------------------------------------
//Dibuja toda la interfaz grafica del programa, con sus respectivos colores,
//textos, fuentes para el texto, anchos de lineas, etc. todo en base a
//coordenadas iniciando en la esquina inferior izquierda (0,0)
//------------------------------------------------------------------------------
void dibujarInterfaz(){
	//-----------------------------------------------
	//se crea la ventana con fondo gris-oscuro-opaco
	SRGP_setColor(4);			//color del fondo de la ventana grafica
	SRGP_fillRectangleCoord(0,0,ANCHO-1,ALTO-1);
	SRGP_setColor(11);
	SRGP_fillRectangleCoord(0,179,ANCHO-1,ALTO-1);
	//-----------------------------------------------
	//dibuja todos los botones principales de la pantalla
	iniciarBoton(590,10,100,25,"SALIR");
	iniciarBoton(480,10,100,25,"ACERCA DE");
	iniciarBoton(10,10,250,25,"CANCELAR OPERACION");
	iniciarBoton(10,45,100,20,"IMPORTAR");
	iniciarBoton(240,45,100,20,"EXPORTAR");
	iniciarBoton(360,45,100,20,"RECORRER");
	iniciarBoton(10,95,100,20,"AGREGAR");
	iniciarBoton(240,95,100,20,"ELIMINAR");
	iniciarBoton(590,10,100,25,"ACEPTAR"); //ES EL DE ACERCA DE...

	//-----------------------------------------------
	//dibuja la linea que divide la ventana
	SRGP_setLineWidth(2);
	SRGP_lineCoord(0,179,ANCHO,179);
	//-----------------------------------------------
	//dibuja todos los cuadros de texto
	iniciarCuadro(15,130,670,20,80);	//pares ordenados
	iniciarCuadro(115,95,120,20,5);	//ingrese un entero
	iniciarCuadro(115,45,120,20,10);	//ingrese nombre del archivo
	iniciarCuadro(465,45,225,20,20);	//recorrido

	//-----------------------------------------------
	SRGP_setFont(4);
	SRGP_setColor(1);
	SRGP_text(SRGP_defPoint(110,81),"( Ingrese un valor entero )");	//escribe el texto "pre - order" dentro del recuadro

	//-----------------------------------------------
	//linea a la mitad
	SRGP_setLineWidth(2);
	SRGP_lineCoord(350,40,350,117);

	//-----------------------------------------------
	//escribe el texto de recuadro de "recorrido"
	SRGP_setLineWidth(1);
	SRGP_setColor(1);
	SRGP_lineCoord(360,75,690,75); //Linea grande abajo
	SRGP_lineCoord(360,75,360,110); //linea izquierda
	SRGP_lineCoord(690,75,690,110); //linea derecha
	SRGP_lineCoord(465,110,690,110); // grande arriba
	SRGP_lineCoord(360,110,365,110); // pequeña
	SRGP_setFont(4);
	SRGP_setColor(3);
	SRGP_text(SRGP_defPoint(370,107),"Tipo de Recorrido");	//escribe el texto "pre - order" dentro del recuadro
	SRGP_setColor(1);
	SRGP_setFont(7);
	SRGP_text(SRGP_defPoint(385,89),"Pre-Order");	// escribe el texto de "In - order" dentro del recuadro
	SRGP_text(SRGP_defPoint(495,89),"In-Order");	//escribe el texto "Pos - order" dentro del recuadro
	SRGP_text(SRGP_defPoint(605,89),"Pos-Order");	//dibuja los circulos de chequeo del recorrido
	iniciarCirculo(370,88,1);	//circulo 1
	iniciarCirculo(480,88,0);	//circulo 2
	iniciarCirculo(590,88,0);	//circulo 3

	//-----------------------------------------------
	//escribe el texto de recuadro de "pares ordenados anidados/edicion"
	SRGP_setLineWidth(1);
	SRGP_setColor(1);
	SRGP_lineCoord(10,125,690,125); //Linea grande abajo
	SRGP_lineCoord(10,125,10,160); //linea izquierda
	SRGP_lineCoord(690,125,690,160); //linea derecha
	SRGP_lineCoord(157,160,690,160); // grande arriba
	SRGP_lineCoord(10,160,15,160); // pequeña
	SRGP_setFont(4);
	SRGP_setColor(3);
	SRGP_text(SRGP_defPoint(20,157),"Pares Ordenados Anidados");	//escribe el texto "pre - order" dentro del recuadro

	//-----------------------------------------------
	SRGP_refresh();		//refresca la interfaz dibujada en la ventana
}


//------------------------------------------------------------------------------
//Funcion que procesa los eventos en el programa, es decir, es la que verifica
//cada 5 sentesimas de segundo si a ocurrido un evento de teclado o mouse
//dentro de la ventana del programa
//------------------------------------------------------------------------------
void procesarEventos(){
	int evento;		//declaracion de la variable local n como entero
	while(1){
		refrescar();	//copia lo dibujado en lienzo en la pantalla
		/*esta funcion retorna LOCATOR O KEYBOARD, dependiendo que dispositivo ha generado evento,
		el numero 5 indica que cada 5*(1/60)seg verificara si a ocurrido un evento*/
		evento = SRGP_waitEvent(5);
		switch(evento){
			case LOCATOR:	procesarRaton();
					break;
			case KEYBOARD:	procesarTeclado();
					break;
		}
	}
}

//------------------------------------------------------------------------------
//La funcion procesar raton tiene como funcion principal verificar si el evento
//que se genero requiere de algun proceso, por ejemplo, con el boton salir,
//esta dibujado en ciertas coordenadas en la ventana y si el evento que se
//genero esta dentro de esas coordenadas entonces se ejecutaran los procesos
//correspondientes, que en este caso seria, destruir la ventana grafica para
//luego salir del programa.
//------------------------------------------------------------------------------
void procesarRaton(){
	int x,y,j,longitud,index=-1,index2=-2,index3=-3;
	char temp[12]; //cadena[4000];
	SRGP_getDeluxeLocator(&dlm);	//guarda toda la informacion  del raton
	if(cuadros[indice].escribir == TRUE)
		return;
	x = dlm.position.x;	//contine la posicion en X del puntero del raton
	y = dlm.position.y;	//contine la posicion en Y del puntero del raton
	index = getIndex(x,y,1);	//verifica y encuentra el indice del boton pulsado
	index2 = getIndex(x,y,2);	//verifica y encuentra el indice del circulo de la clave de ordenacion
	index3 = getIndex(x,y,3);	//verifica y encuentra el indice del cuadro de texto
	if(BAD == TRUE){	//si BAD es verdadero solo verifica si se presiona el boton aceptar
		if(dlm.button_chord[LEFT_BUTTON] == DOWN)	//clic-izquiedo-abajo
			if(x>585 && x<685 && y>189 && y<219){	//si fue el boton aceptar
				dibujarArbolB();
				BAD = FALSE;	//y a BAD lo hace falso, para que todo siga normal
			}
		return;
	}
	if(dlm.button_chord[LEFT_BUTTON] == DOWN){
		switch(index){
			case 0:	dibujarBoton(index,1);	//boton salir
				break;
			case 1: 	dibujarBoton(index,1); //boton acerca de
						break;
			case 2: 	dibujarBoton(index,1); //boton cancelar operacion
						break;
			case 3:	 dibujarBoton(index,1); //boton importar
						break;
			case 4: 	dibujarBoton(index,1); //boton exportar
						break;
			case 5: 	dibujarBoton(index,1); //boton recorrer
						break;
			case 6: 	dibujarBoton(index,1); //boton agregar
						break;
			case 7: 	dibujarBoton(index,1); //boton Eliminar
						break;
		}
		if(index2!=-2){	//si fue un indice de un circulo
			for(j=0;j<NCirculos;j++){	//recorre todos los indices de los circulos
				if(j == index2)		//si j fue el circulo presionado
					dibujarCirculo(j,1);	//dibuja el circulo pulsado
				else
					dibujarCirculo(j,0);	//dibuja los otros normales
			}
		}
		if(index3!=-3){	//si fue un cuadro de texto
			if(index3 == 3)	//verifico si fue uno de los cuadros no modificados
				return;
			indice = index3;	//asigno al indice global el indice de la cuadro presionado
			dibujarCuadro(index3,1);	//dibuja el cuadro resaltado de los demas
			cuadros[index3].escribir = TRUE;	//hago verdadero escribir en el cuadro
			//se crea el lienzo temporal correspondiente al cuadro de texto
			free(cuadros[indice].cadena);
			longitud = cuadros[indice].longitud;
			if(!(cuadros[indice].cadena = (char*)malloc(longitud))){
				printf("Insuficiente memoria...\n");
				SRGP_end();
				exit(0);
			}
			lienzoText = SRGP_createCanvas(cuadros[index3].ancho,cuadros[index3].alto);
		}
	}
	if(dlm.button_chord[LEFT_BUTTON] == UP){	//clic-izquierdo-arriba
		switch(index){	//verifica cual boton fue el pulsado
			case 0:	SRGP_end();	//el boton salir, ya no se dibuja
				exit(0);	//solo se destruye la ventana grafica y se sale
				break;
			case 1:	dibujarBoton(index,0); //boton acerca de
						acercaDe();
						//a la variable BAD (BotonAcercaDe) le asignamos verdadero
						BAD = TRUE;
						break;
			case 2: 	dibujarBoton(index,0); //boton cancelar operacion
						break;

			case 3: 	dibujarBoton(index,0); //boton cancelar importar
						importaArchivo(archivoIE);
						break;

			case 4: dibujarBoton(index,0); //boton exportar
						exportaArchivo(archivoIE);
						break;

			case 5: 	dibujarBoton(index,0); //boton recorrer
						break;
			case 6: 	dibujarBoton(index,0); //boton agregar
						insertar(&T,xxx);
						clear();
						dibujar(T,315,575,6,1,1);
						mostrar(T);
						break;
			case 7: 	dibujarBoton(index,0); //boton Eliminar
						eliminar(&T,xxx); /*ENTRA EN UN ERROR FATAL... PESIMO HORRIBLE..!!!!!!!!!!!!!!!!!!*/
						clear();
						dibujar(T,315,575,6,1,1);
						break;
		}
	}
}

//------------------------------------------------------------------------------
//Funcion que se encarga de procesar todos los eventos generados por el teclado,
//en esta funcion se revisan las especificaciones de escritura, etc.
//El boton "Enter"(ASCII 13) maneja todas las validaciones de cada uno de los
//cuadros de texto.
//------------------------------------------------------------------------------

void procesarTeclado(){
	char c,temp[100];
	point punto,punto1;
	int longitud;
	SRGP_getDeluxeKeyboard(&dkm);		//guarda toda la informacion del teclado
	if(cuadros[indice].escribir == FALSE)
		return;
	SRGP_useCanvas(lienzoText);
	c = dkm.buffer[0];
	switch(c){
		/*tecla ENTER*/
		case 13:	SRGP_useCanvas(lienzo);	//vuelvo al lienzo temporal principal
				dibujarCuadro(indice,0);	//dibuja el cuadro de texto normal
				SRGP_setFont(0);	//carga la fuente que esta por defauld
				punto1 = SRGP_defPoint(cuadros[indice].posicion.x+4,cuadros[indice].posicion.y+5);
				SRGP_text(punto1,cuadros[indice].cadena);	//imprime la cadena en la caja sin "|"
				SRGP_setFont(7);	//regresa el tipo de letra por default
				cuadros[indice].escribir = FALSE;	//asigna a escribir FALSE
				SRGP_deleteCanvas(lienzoText);	//borra el lienzo ocupado para el cuadro de texto
				if(indice == 3)
					return;
				//borra lo digitado en el arreglo cadena, mas adelante puede sufrir cambios
				if(indice == 2)
					sprintf(archivoIE,"%s",cuadros[indice].cadena);
				if(indice == 0)
					sprintf(cat,"%s",cuadros[indice].cadena);
				if(indice == 1)
					xxx = atoi(cuadros[indice].cadena);
				free(cuadros[indice].cadena);
				longitud = cuadros[indice].longitud;
				if(!(cuadros[indice].cadena = (char*)malloc(longitud))){
					printf("Insuficiente memoria...\n");
					SRGP_end();
					exit(0);
				}
				return;
		/*tecla TAB*/
		case 9:		SRGP_beep();
				break;
		/*tecla retroceso o suprimir*/
		case 8:	if(strlen(cuadros[indice].cadena) == 0)
					SRGP_beep();	//genera un sonido de beep en el sistema
				else{
					cuadros[indice].cadena[strlen(cuadros[indice].cadena)-1] = '\0';
					sprintf(temp,"%s|",cuadros[indice].cadena);
					clearText();	//limpia el lienzo temporal del texto
					SRGP_setColor(1);
					SRGP_text(SRGP_defPoint(4,5),temp);	//coloca el texto en lienzoText
				}
				break;
		/*tecla ESC*/
		case 27:	SRGP_beep();
				break;
		/*si no es ninguna de esas teclas anteriores entonces escribe*/
		default:	if(strlen(cuadros[indice].cadena) < (cuadros[indice].longitud)){
					sprintf(cuadros[indice].cadena,"%s%c",cuadros[indice].cadena,c);
					sprintf(temp,"%s|",cuadros[indice].cadena);
					clearText();	//limpia el lienzo temporal del texto
					SRGP_setColor(1);
					SRGP_text(SRGP_defPoint(4,5),temp);	//coloca el texto en lienzoText
				}
				else
					SRGP_beep();
				break;
	}//cierre del switch
	SRGP_useCanvas(lienzo);
	punto = SRGP_defPoint(cuadros[indice].posicion.x+2,cuadros[indice].posicion.y+2);
	SRGP_copyPixel(lienzoText,SRGP_defRectangle(2,2,cuadros[indice].ancho-2,cuadros[indice].alto-2),punto);
	SRGP_useCanvas(lienzoText);
}

//------------------------------------------------------------------------------
//funcion que limpia la lona activa con un rectangulo blanco, para eso la funcion
//se auxilia de dos funciones de SRGP para el manejo de lienzos, la funcion
//SRGP_inquireCanvasExtent, devuelve un rectangulo con el tamaÃ±o de un lienzo
//el cual recibe como parametro; SRGP_inquireActiveCanvas devuelve la lona
//actualmente activa.
//------------------------------------------------------------------------------
void clearText(void){
	SRGP_setColor(0);	//color blanco
	SRGP_fillRectangle(SRGP_inquireCanvasExtent(SRGP_inquireActiveCanvas()));
	SRGP_setColor(1);	//color negro
}


//------------------------------------------------------------------------------
//Funcion que carga todos los colores declarados en el arreglo colorNames para
//poder ser utilizados por la libreria SRGP para ser implementados en la
//interfaz grafica.
//------------------------------------------------------------------------------
void definirColores(void){
	int i;
	for(i=0;i<NCOLORS;i++)
		SRGP_loadCommonColor(i,colorNames[i]);
}

//------------------------------------------------------------------------------
//Funcion que se encarga de definir todas las fuentes que se van a utilizar en
//la interfaz grafica del programa, el indice de cada fuente inicia desde el
//numero 1 ya que existe una fuente por defauld asignada para el indice 0
//------------------------------------------------------------------------------
void definirFuentes(){
	int i;
	for(i=1;i<=NFONTS;i++)
		SRGP_loadFont(i,fontNames[i-1]);
}

//------------------------------------------------------------------------------
//Funcion que inicia los botones, es decir que dibuja inicialmente los botones
//y al mismo tiempo almacena la informacion del boton creado para identificarlo
//con un indice luego.
//------------------------------------------------------------------------------
void iniciarBoton(int x,int y,int ancho,int alto,char *nombre){
	//Almacena la informacion enviada como parametros al arreglo botones segun su estructura e index
	botones[NBotones].pi.x = x;
	botones[NBotones].pi.y = y;
	botones[NBotones].pf.x = x+ancho;
	botones[NBotones].pf.y = y+alto;
	botones[NBotones].nombre = nombre;
	if(NBotones != 8)
		dibujarBoton(NBotones,0);	//dibuja el boton
	NBotones++;			//aumenta el contador del numero de botones
}

//------------------------------------------------------------------------------
//Esta funcion se encarga de dibujar los botones en la ventana, recibe como
//parametros dos valores enteros, el primero indica un indice donde le indica
//a la funcion la informacion que puede tomar del arreglo global declarado
//llamado "botones" y un ultimo parametro p que le indica a la funcion si el
//boton sera dibujado presionado o no, si se desea dibujar presionado debe de
//enviarse un valor igual a 1, de lo contrario el programa dibujara el boton
//no precionado, para lo que en el codigo se utilizara un valor igual a 0
//------------------------------------------------------------------------------
void dibujarBoton(int index,int p){
	int x1,y1,x2,y2,anch,asc,desc,espX,espY;
	char cad[50];
	//almacena en las variables toda la informacion referente al boton segun su index
	x1 = botones[index].pi.x;
	y1 = botones[index].pi.y;
	x2 = botones[index].pf.x;
	y2 = botones[index].pf.y;
	SRGP_setFont(1);
	sprintf(cad,"%s",botones[index].nombre);	//copia el nombre del boton en cad
	/*esta funcion nos brinda informacion del tamano de la cadena que es enviada como primer
	parametro, y regresa sus dimensiones en los tres punteros restantes*/
	SRGP_inquireTextExtent(cad,&anch,&asc,&desc);
	if(p == 1){			//dibuja el boton presionado
		SRGP_setColor(11);
		SRGP_fillRectangleCoord(x1,y1,x2,y2);
		dibujarEfecto(x1,y1,x2,y2,2,1);
		//calcula las coordenadas para que el texto sea centrado
		espX = x1+((x2-x1-anch)/2)+1;
		espY = y1+((y2-y1-asc+desc)/2)-1;
		//escribe el texto sobre el boton
		SRGP_setColor(1);			//color negro
		SRGP_text(SRGP_defPoint(espX,espY),cad);
	}
	else{				//dibuja el boton normal
		SRGP_setColor(4);
		SRGP_fillRectangleCoord(x1,y1,x2,y2);
		dibujarEfecto(x1,y1,x2,y2,2,0);
		//calcula las coordenadas para que el texto sea centrado
		espX = x1+((x2-x1-anch)/2);
		espY = y1+((y2-y1-asc+desc)/2);
		//escribe el texto sobre el boton
		SRGP_setColor(1);			//color negro
		SRGP_text(SRGP_defPoint(espX,espY),cad);
	}
	//restableciendo el ancho de linea por defecto
	SRGP_setLineWidth(1);
}

//------------------------------------------------------------------------------
//funcion que de acuerdo a x,y retorna el indice del boton, celda, circulo o
//cuadro de texto en el que se ha clickeado o -1, -2, -3, -4 respectivamente si
//no es ninguno; la traduccion de "getIndex" al espanol es "obtenerIndice".
//------------------------------------------------------------------------------
int getIndex(int x,int y,int q){
	int i = 0;
	switch(q){
		case 1:	for(i=0;i<NBotones;i++){	//recorre todo el arreglo global botones
			if(x > botones[i].pi.x && x < botones[i].pf.x && y > botones[i].pi.y && y < botones[i].pf.y)
				return i;
			}
			return -1; // indica que fracaso la busqueda
		case 2:	for(i=0;i<NCirculos;i++){		//recorre todo el arreglo global circulos
			if(x > circulos[i].pi.x && x < circulos[i].pf.x && y > circulos[i].pi.y && y < circulos[i].pf.y)
				return i;
			}
			return -2; // indica que fracaso la busqueda
		case 3:	for(i=0;i<NCuadros;i++){		//recorre todo el arreglo global cuadros
			if(x > cuadros[i].posicion.x && x < (cuadros[i].posicion.x+cuadros[i].ancho))
				if(y > cuadros[i].posicion.y && y < (cuadros[i].posicion.y+cuadros[i].alto))
					return i;
			}
			return -3; // indica que fracaso la busqueda*/
	}
}

//------------------------------------------------------------------------------
//Esta funcion como su nombre lo indica dibuja el efecto de relieve o
//profundida dependiendo del valor proporcionado a p como parametro a la
//funcion de los botones, celdas, rectangulos o cualquier cosa que amerite ese
//efecto en la ventana, recibe inicialmente el punto inicial xi,yi y luego el
//punto final xf,yf, lurgo pide el grosor de la linea que tendra la profundidad
//o el relieve.
//------------------------------------------------------------------------------
void dibujarEfecto(int xi,int yi,int xf,int yf,int grosorLinea,int p){
	SRGP_setLineWidth(grosorLinea);
	if(p == 1){				//dibuja las lineas con el efecto de profundidad
		SRGP_setColor(10);		//color gris claro
		SRGP_lineCoord(xi,yi,xf,yi);	//linea de abajo
		SRGP_lineCoord(xf,yi,xf,yf);	//linea derecha
		SRGP_setColor(1);		//color negro
		SRGP_lineCoord(xi,yi,xi,yf);	//linea izquierda
		SRGP_lineCoord(xi,yf,xf,yf);	//linea de arriba
	}
	else{					//dibuja las lineas con el efecto de relieve
		SRGP_setColor(1);		//color negro
		SRGP_lineCoord(xi,yi,xf,yi);	//linea de abajo
		SRGP_lineCoord(xf,yi,xf,yf);	//linea derecha
		SRGP_setColor(10);		//color gris claro
		SRGP_lineCoord(xi,yi,xi,yf);	//linea izquierda
		SRGP_lineCoord(xi,yf,xf,yf);	//linea de arriba
	}
}

//------------------------------------------------------------------------------
//funcion que dibuja los circulos de chequeo de la clave actual de ordenacion.
//recibe como parametros primeramente el indice el cual indica en que posicion
//del arreglo circulos en cual esta declarado y mantiene la informacion necesaria
//para dibujarse el circulo, luego verifica si debe dibujarse con un cheque o no.
//------------------------------------------------------------------------------
void dibujarCirculo(int index,int chequeo){
	int x1,y1,x2,y2;
	//almacena en las variables locales la informacion del circulo segun su index
	x1 = circulos[index].pi.x;
	y1 = circulos[index].pi.y;
	x2 = circulos[index].pf.x;
	y2 = circulos[index].pf.y;
	//dibuja el circulo de chequeo de cada clave
	SRGP_setLineWidth(1);
	SRGP_setColor(0);					//color blanco
	SRGP_fillEllipse(SRGP_defRectangle(x1,y1,x2,y2)); 	//circulo relleno
	SRGP_setColor(1);					//color negro
	SRGP_ellipse(SRGP_defRectangle(x1,y1,x2,y2)); 		//marco del circulo
	if(chequeo == 1)
		SRGP_fillEllipse(SRGP_defRectangle(x1+1,y1+1,x2-1,y2-1)); 	//circulo relleno
}

//------------------------------------------------------------------------------
//Dibuja inicialmente los circulos de la clave actua de ordenacion pero primero
//almacena la informacion que recibe como paramentros en el arreglo circulos
//en base a un indice.
//------------------------------------------------------------------------------
void iniciarCirculo(int x,int y,int chequeo){
	//Almacena la informacion enviada como parametros al arreglo circulos segun su estructura e index
	circulos[NCirculos].pi.x = x;
	circulos[NCirculos].pi.y = y;
	circulos[NCirculos].pf.x = x+9;
	circulos[NCirculos].pf.y = y+9;
	dibujarCirculo(NCirculos,chequeo);	//dibuja el circulo
	NCirculos++;			//aumenta el contador del numero de circulos
}

//------------------------------------------------------------------------------
//funcion que inicializa una caja de texto y lo que hace es almacenar la info
//obtenida como parametros a esta funcion y la almacena en base a la estructura
//cuadro en el arreglo global cuadros, para luego dibujar ese cuadro de texto
//con una llamada a la funcion dibujarCuadro.
//------------------------------------------------------------------------------
void iniciarCuadro(int x,int y,int ancho,int alto,int longitud){
	//Almacena la informacion enviada como parametros al arreglo cuadros segun su estructura e index
	cuadros[NCuadros].posicion = SRGP_defPoint(x,y);
	cuadros[NCuadros].ancho = ancho;
	cuadros[NCuadros].alto = alto;
	if(!(cuadros[NCuadros].cadena = (char*)malloc(longitud))){	//asigna memoria para la cadena
		printf("Insuficiente memoria...\n");
		SRGP_end();	//destruye la ventana grafica generada por SRGP_begin
		exit(0);
	}
	cuadros[NCuadros].cadena[0] = '\0';
	cuadros[NCuadros].longitud = longitud;
	cuadros[NCuadros].escribir = FALSE;
	dibujarCuadro(NCuadros,0);	//dibuja el cuadro de texto
	NCuadros++;			//aumenta el contador del numero de cuadros de texto
}

//------------------------------------------------------------------------------
//funcion que dibuja un cuadro de texto en base a la informacion introducida en
//la funcion iniciarCuadro, recibe como parametros su indice y un valor entero
//para saber si se dibujara resaltado o no.
//------------------------------------------------------------------------------

void dibujarCuadro(int index,int chequeo){
	int x,y,anch,alt;	//variables locales de la funcion
	//almacena en las variables locales la informacion del cuadro de texto segun su index
	x = cuadros[index].posicion.x;
	y = cuadros[index].posicion.y;
	anch = cuadros[index].ancho;
	alt = cuadros[index].alto;
	//dibuja el cuadro de texto
	SRGP_setColor(4);
	SRGP_fillRectangleCoord(x-2,y-2,x+anch+2,y+alt+2);
	SRGP_setColor(0);				//color blanco
	SRGP_fillRectangleCoord(x,y,x+anch,y+alt);	//cuadro de texto relleno
	if(chequeo == 1)	//depende de chequeo para cambiar el tipo de linea
		SRGP_setLineWidth(3);	//grosor de linea (3 pixeles)
	else
		SRGP_setLineWidth(1);	//grosor de linea (1 pixel)
	SRGP_setColor(1);			//color negro
	SRGP_rectangleCoord(x,y,x+anch,y+alt);	//marco del cuadro de texto
	SRGP_setLineWidth(1);	//reestablece el grosor de linea por defauld
}

//------------------------------------------------------------------------------
//funcion que crea un lienzo temporal y dibuja en el todo lo que debe contener
//sobre la informacion del programa, para uso exclusivo del boton acerca de..
//no recibe nada como parametros, simplemente lo muestra y ya.
//------------------------------------------------------------------------------
void acercaDe(void){
	int x,y;
	lienzoAcerca = SRGP_createCanvas(ANCHO-1,490);
	SRGP_useCanvas(lienzoAcerca);
	SRGP_setColor(11);
	SRGP_fillRectangleCoord(0,0,ANCHO-1,490);
	dibujarLogoDEI(630,431);
	dibujarLogoUCA(630,321);
	SRGP_setColor(1);
	SRGP_setFont(1);
	SRGP_text(SRGP_defPoint(100,440),"Universidad Centroamericana");
	SRGP_text(SRGP_defPoint(100,425),"'Jose Simeon Canas'");
	SRGP_text(SRGP_defPoint(100,410),"Depto. de Electronica e Informatica");
	SRGP_text(SRGP_defPoint(100,260),"Fecha:  01 / 07 / 2006");
	SRGP_text(SRGP_defPoint(100,210),"Nombre del Grupo:  FACEB");
	SRGP_text(SRGP_defPoint(100,145),"<> Carmen Elena Constanza Rodríguez   ");
	SRGP_text(SRGP_defPoint(100,130),"<> Fany Vanessa Melendez Duenas       ");
	SRGP_text(SRGP_defPoint(100,115),"<> Ana Ruth Galdamez Aguirre          ");
	SRGP_text(SRGP_defPoint(100,100),"<> Veronica Carolina Bautista Huezo   ");
	SRGP_text(SRGP_defPoint(360,145),"     elenita129@hotmail.com");
	SRGP_text(SRGP_defPoint(360,130),"     fanyvanessam@gmail.com");
	SRGP_text(SRGP_defPoint(360,115),"     ruthgaldamez_12@yahoo.com");
	SRGP_text(SRGP_defPoint(360,100),"     vbfemni@yahoo.es");
	SRGP_setColor(3);	//azul oscuro
	SRGP_text(SRGP_defPoint(100,160),"Creadores");
	SRGP_text(SRGP_defPoint(360,160),"       Correo / E-mail");
	SRGP_setColor(1);
	SRGP_setFont(11);
	SRGP_text(SRGP_defPoint(100,335),"ARBOL DE BUSQUEDA BINARIO");
	SRGP_setColor(1);
	//dibuja el boton Aceptar
	dibujarBoton(8,0);
	//copia en liezo lo antes hecho
	SRGP_useCanvas(lienzo);	//pone en uso el lienzo que se refresca en la pantalla
	SRGP_copyPixel(lienzoAcerca,SRGP_defRectangle(0,0,ANCHO,490),SRGP_defPoint(0,179));
	SRGP_deleteCanvas(lienzoAcerca);	//destruye el lienzo creado para Acerca de...
}

//------------------------------------------------------------------------------
//Dibuja el logo del Departamento de Electronica e Informatica de la UCA,
//recibe como parametro el punto inicial donde se desea que se dibuje
//------------------------------------------------------------------------------
void dibujarLogoDEI(int x,int y){
	SRGP_setColor(3);
	SRGP_fillRectangleCoord(x,y,x+60,y+30);
	SRGP_setLineWidth(2);
	//dibujando lineas blancas del interior del rectangulo relleno azul
	SRGP_setColor(0);
	SRGP_lineCoord(x+28,y+2,x+28,y+25);
	SRGP_lineCoord(x+3,y+5,x+44,y+5);
	SRGP_lineCoord(x+3,y+14,x+44,y+14);
	SRGP_lineCoord(x+33,y+10,x+55,y+10);
	SRGP_lineCoord(x+4,y+5,x+4,y+14);
	SRGP_lineCoord(x+53,y+12,x+53,y+22);
	SRGP_ellipse(SRGP_defRectangle(x+51,y+22,x+55,y+26));
 	//dibujando las lineas exteriores de arriba
	SRGP_setColor(3);
	SRGP_lineCoord(x+6,y+30,x+6,y+35);
	SRGP_lineCoord(x+14,y+30,x+14,y+35);
	SRGP_lineCoord(x+22,y+30,x+22,y+35);
	SRGP_lineCoord(x+30,y+30,x+30,y+35);
	SRGP_lineCoord(x+38,y+30,x+38,y+35);
	SRGP_lineCoord(x+46,y+30,x+46,y+35);
	SRGP_lineCoord(x+54,y+30,x+54,y+35);
	//dibujando las lineas exteriores de abajo
	SRGP_lineCoord(x+6,y,x+6,y-5);
	SRGP_lineCoord(x+14,y,x+14,y-5);
	SRGP_lineCoord(x+22,y,x+22,y-5);
	SRGP_lineCoord(x+30,y,x+30,y-5);
	SRGP_lineCoord(x+38,y,x+38,y-5);
	SRGP_lineCoord(x+46,y,x+46,y-5);
	SRGP_lineCoord(x+54,y,x+54,y-5);
	//dibujando las circunferencias de las lineas exteriores de arriba
	SRGP_ellipse(SRGP_defRectangle(x+4,y+35,x+8,y+39));
	SRGP_ellipse(SRGP_defRectangle(x+12,y+35,x+16,y+39));
	SRGP_ellipse(SRGP_defRectangle(x+20,y+35,x+24,y+39));
	SRGP_ellipse(SRGP_defRectangle(x+28,y+35,x+32,y+39));
	SRGP_ellipse(SRGP_defRectangle(x+36,y+35,x+40,y+39));
	SRGP_ellipse(SRGP_defRectangle(x+44,y+35,x+48,y+39));
	SRGP_ellipse(SRGP_defRectangle(x+52,y+35,x+56,y+39));
	//dibujando las circunferencias de las lineas exteriores de abajo
	SRGP_ellipse(SRGP_defRectangle(x+4,y-9,x+8,y-5));
	SRGP_ellipse(SRGP_defRectangle(x+12,y-9,x+16,y-5));
	SRGP_ellipse(SRGP_defRectangle(x+20,y-9,x+24,y-5));
	SRGP_ellipse(SRGP_defRectangle(x+28,y-9,x+32,y-5));
	SRGP_ellipse(SRGP_defRectangle(x+36,y-9,x+40,y-5));
	SRGP_ellipse(SRGP_defRectangle(x+44,y-9,x+48,y-5));
	SRGP_ellipse(SRGP_defRectangle(x+52,y-9,x+56,y-5));
	//restableciendo colores y anchos de linea por defecto
	SRGP_setLineWidth(1);
	SRGP_setColor(1);
}

//------------------------------------------------------------------------------
//funcion que dibuja el logo de la Universidad Centroamericana "Jose simeon
//Canas" (UCA), la funcion recibe coma parametros dos valores enteros que
//corresponden a las coordenadas x,y donde se desea colocar el logo.
//------------------------------------------------------------------------------
void dibujarLogoUCA(int x,int y){
	point pts[6], pts1[5];		//arreglos local para los vertices de los poligonos
	SRGP_setLineWidth(1);		//grosor de linea 1 pixel
	SRGP_setColor(1);		//color negro
	SRGP_fillRectangleCoord(x,y+25,x+25,y+50);	//dibuja el rectangulo relleno negro
	SRGP_setColor(0);
	SRGP_fillRectangleCoord(x,y,x+25,y+25);		//dibuja el rectangulo relleno blanco
	SRGP_fillRectangleCoord(x+25,y,x+50,y+25);	//dibuja el rectangulo relleno blanco
	SRGP_fillRectangleCoord(x+25,y+25,x+50,y+50);	//dibuja el rectangulo relleno blanco
	SRGP_setColor(1);
	SRGP_setLineWidth(2);
	SRGP_rectangleCoord(x,y,x+50,y+50);	//dibuja el rectangulo grande
	SRGP_lineCoord(x+38,y+35,x+38,y+65);	//dibuja la linea vertical de la cruz
	SRGP_lineCoord(x+33,y+60,x+43,y+60);	//dibuja la linea horizontal de la cruz
	SRGP_setLineWidth(1);
	/*almacena las coordenadas de los vertices del poligono*/
	pts[0] = SRGP_defPoint(x+3,y+20);	//punto 1 del poligono
	pts[1] = SRGP_defPoint(x+8,y+38);	//punto 2 del poligono
	pts[2] = SRGP_defPoint(x+11,y+44);	//punto 3 del poligono
	pts[3] = SRGP_defPoint(x+18,y+42);	//punto 4 del poligono
	pts[4] = SRGP_defPoint(x+11,y+25);	//punto 5 del poligono
	pts[5] = SRGP_defPoint(x+10,y+22);	//punto 6 del poligono
	/*---------------------------------------------*/
	SRGP_setColor(0);			//color blanco
	SRGP_fillPolygon(6,pts);		//dibuja el poligono relleno
	SRGP_fillEllipseArc(SRGP_defRectangle(x-10,y+23,x+17,y+48),306,371);	//arco mas grande
	SRGP_setColor(1);			//color negro
	/*dibuja un poligono para cubrir partes blancas*/
	pts1[0] = SRGP_defPoint(x+5,y+25);
	pts1[1] = SRGP_defPoint(x+8,y+38);
	pts1[2] = SRGP_defPoint(x+11,y+44);
	pts1[3] = SRGP_defPoint(x,y+44);
	pts1[4] = SRGP_defPoint(x,y+25);
	SRGP_fillPolygon(5,pts1);
	/*---------------------------------------------*/
	SRGP_ellipseArc(SRGP_defRectangle(x+7,y+34,x+11,y+38),315,495);	//dibuja el arco interior
	SRGP_lineCoord(x+3,y+20,x+8,y+38);	//dibuja la linea izquierda del buho
	SRGP_lineCoord(x+3,y+20,x+10,y+22);	//dibuja la linea de abajo del buho
	SRGP_lineCoord(x+10,y+22,x+11,y+25);	//dibuja la linea derecha del buho
	SRGP_fillEllipseArc(SRGP_defRectangle(x+11,y+43,x+19,y+50),205,310);	//arco superior
	SRGP_ellipse(SRGP_defRectangle(x+11,y+40,x+13,y+42));	//ojo izquierdo del buho
	SRGP_ellipse(SRGP_defRectangle(x+15,y+39,x+17,y+41));	//ojo derecho del buho
	SRGP_lineCoord(x+13,y+38,x+13,y+39);	//dibuja la linea izquierda de la nariz del buho
	SRGP_setLineWidth(2);
	SRGP_lineCoord(x+25,y,x+25,y+50);	//dibuja la linea vertical mayor
	SRGP_lineCoord(x,y+25,x+50,y+25);	//dibuja la linea horizontal mayor
	SRGP_setLineWidth(1);
	SRGP_lineCoord(x+4,y+23,x+10,y+34);	//linea interna del ala del buho
	/*dibuja las letras UCA*/
	SRGP_setLineWidth(3);			//grosor de linea 2 pixeles
	SRGP_lineCoord(x+27,y+15,x+27,y+24);	//linea izquierda de la letra U
	SRGP_lineCoord(x+34,y+15,x+34,y+24);	//linea derecha de la letra U
	SRGP_ellipseArc(SRGP_defRectangle(x+27,y+13,x+34,y+17),180,360);	//dibuja el arco de la U
	SRGP_lineCoord(x+35,y+10,x+35,y+16);	//linea vertical de la letra C
	SRGP_ellipseArc(SRGP_defRectangle(x+35,y+13,x+42,y+20),0,180);	//dibuja el arco de arriba de la C
	SRGP_ellipseArc(SRGP_defRectangle(x+35,y+8,x+42,y+15),180,360);	//dibuja el arco de abajo de la C
	SRGP_lineCoord(x+44,y+2,x+48,y+2);	//linea inferior de la letra A
	SRGP_lineCoord(x+44,y+7,x+48,y+7);	//linea central de la letra A
	SRGP_lineCoord(x+43,y+13,x+48,y+13);	//linea superior de la letra A
	SRGP_lineCoord(x+48,y+2,x+48,y+13);	//linea vertical de la letra A
	SRGP_ellipseArc(SRGP_defRectangle(x+42,y+2,x+45,y+7),90,270);	//dibuja el arco de la letra A
	SRGP_setLineWidth(1);			//restablece el grosor de la liena a 1 pixel
}

//------------------------------------------------------------------------------
//funcion que copia la ventana grafica y hace posible que al minimizar la
//pantalla o trasponerla con otra esta se mantega siempre visible
//------------------------------------------------------------------------------

void refrescar(void){
	SRGP_useCanvas(0);	//lienzo principal por defauld
	//Este procedimiento permite que una porcion de un lienzo sea copiada en otra parte de el mismo o en otro lienzo.
	SRGP_copyPixel(lienzo,SRGP_defRectangle(0,0,ANCHO-1,ALTO-1),SRGP_defPoint(0,0));
	SRGP_useCanvas(lienzo);	//pone en uso nuevamente el lienzo del refresco en pantalla
}


void dibujarArbolB(void){
	clear();
}

//------------------------------------------------------------------------------
//como su nombre lo indica, la funcion limpia la pantalla pero solo en las
//coordenadas (0,179) hasta (ANCHO-1,ALTO-1), en otras palabras solo limpia la
//zona donde se dibuja el tablero de juego.
//------------------------------------------------------------------------------

void clear(void){
	SRGP_setColor(11);	//color de fondo del area de juego
	SRGP_fillRectangleCoord(0,179,ANCHO-1,ALTO-1);
	SRGP_refresh();
}

//-------------------------------------------------------------------------------------------
//funcion utilizada para  enviar (exportar) de un archivo
//-------------------------------------------------------------------------------------------

void importaArchivo(char *cadena){
	FILE *f;
	char frase[80],frase2[80],frase3[80];
	int n,m;
		
		f=fopen(cadena,"r");
		if(f==NULL) {
			SRGP_setFont(1);
			SRGP_text(SRGP_defPoint(260,440),"El archivo no se pudo abrir");
			return;
		}else{
		fgets(frase,78,f);
		n=strlen(frase);
		if(frase[n-1]=='\n') frase[n-1]='\0';
		SRGP_text(SRGP_defPoint(20,135),frase);
		//iniciarCuadro(15,130,670,20,80);	//pares ordenados
		fclose(f);
	}

}

void exportaArchivo(char *cadena){
	FILE *f;
	char frase[80],frase2[80],frase3[80];
	int n,m,x,y,x1,y1;
		
		f=fopen(cadena,"w+");
		if(f==NULL) {
			SRGP_setFont(1);
			SRGP_text(SRGP_defPoint(260,440),"El archivo no existe");
			return;
		}else{
			fputs(cat,f);
			fclose(f);
		}

}
//--------------------------------------------------------------------------------------------------
// funciones para la utilizacion de nodos
//--------------------------------------------------------------------------------------------------


Nodo *crear(void){
	Nodo *temp=NULL;
	temp = (Nodo *)malloc(sizeof(Nodo));
	if(temp) temp->izq=temp->der=NULL;
	return temp;
}

//---------------------------------------------------------------------------
// funcion para insertar un nodo
//--------------------------------------------------------------------------
void insertar(Nodo **T,int item){
	Nodo *temp=NULL;
	temp=crear();
	if(temp==NULL)
		return;
	temp->item = item;
	insertarAux(T,temp);
}

void insertarAux(Nodo **T,Nodo *temp){
	if(*T==NULL){
		*T=temp;
		return;
	}
	if(temp->item <= (*T)->item)
		insertarAux(&((*T)->izq),temp);
	else
		insertarAux(&((*T)->der),temp);
}


//---------------------------------------------------------------------------
// funcion para verificar el mayor nodo
//--------------------------------------------------------------------------
int mayorNodo(Nodo *T){
	if(T->der==NULL)
		return T->item;
	else
		return mayorNodo(T->der);
}

//---------------------------------------------------------------------------
// funcion para eliminar un nodo
//--------------------------------------------------------------------------
void eliminar(Nodo **T,int item){
	Nodo *temp=NULL;
	int mayor;
	if(*T==NULL)
		return;
	if((*T)->item == item){ // Pre-Order: iniciamos visitando la raíz
		// Caso 1: nodo hoja
		if((*T)->izq == (*T)->der){
			free(*T);
			*T=NULL;
			return;
		}
		// Caso 2: solo un hijo
		if((*T)->der==NULL){  // Solo hijo izquierdo
			temp=*T;
			*T=(*T)->izq;
			free(temp);
			return;
		}
		if((*T)->izq==NULL){  // Solo hijo derecho
			temp=*T;
			*T=(*T)->der;
			free(temp);
			return;
		}
		// Caso 3: los dos hijos presentes
		mayor = mayorNodo((*T)->izq);
		eliminar(T,mayor);
		(*T)->item = mayor;
		return;
	}
	// Se recorre la rama izquierda o la derecha
	if(item < (*T)->item)
		eliminar(&((*T)->izq),item);
	else
		eliminar(&((*T)->der),item);
}

//-------------------------------------------------------------------------------
// funcion para mostrar los elementos del arbol con sus respectivos parentesis
//------------------------------------------------------------------------------
void mostrar(Nodo *T){
	char temp[100],val[50];
	if(T==NULL){
		sprintf(temp,"()"); //printf("()");
		return;
	}
	//if()
	val=atol(T->item);
	sprintf(temp,"%s (%s ",temp,val); //printf("(%d ",T->item);
	mostrar(T->izq);
	mostrar(T->der);
	sprintf(temp,")"); //printf(")");
}


//-------------------------------------------------------------------------------
// funcion para eliminar todos los elementos del arbol
//------------------------------------------------------------------------------
void eliminarTodo(Nodo **T){
	if(*T==NULL) return;
	if((*T)->izq) eliminarTodo(&((*T)->izq));
	if((*T)->der) eliminarTodo(&((*T)->der));
	free(*T);
	*T=NULL;
}

//-------------------------------------------------------------------------------
// funcion para pasar de los parentesis anidados a modo grafico del arbol
//------------------------------------------------------------------------------
void construye(char *cad,Nodo **T){
	if(*T!=NULL) eliminarTodo(T);
	construyeAux(cad,T,0,strlen(cad)-1);
}

//-------------------------------------------------------------------------------
// funcion auxiliar para pasar de los parentesis anidados a modo grafico del arbol
//------------------------------------------------------------------------------
void construyeAux(char *cad,Nodo **T,int I,int F){
	int raiz,II,FI,ID,FD;
	if(I+1==F && cad[I]=='(' && cad[F]==')')	return;
	else
		if(determinaFronteras(&raiz,&II,&FI,&ID,&FD,cad,I,F)){
			insertar(T,raiz);
			construyeAux(cad,T,II,FI);
			construyeAux(cad,T,ID,FD);
		}
		else{
		 	printf("La cadena estaba mal construida!!\n");
		 	return;
		}
}

//-------------------------------------------------------------------------------
// funcion para determinar si el elemento es raiz, rama izq o rama der lo hace
// verificando si existe un espacio entre parentesis y el recorrido es en 
// pre-order es decir 1º raiz 2º izq 3º der
//------------------------------------------------------------------------------
int determinaFronteras(int *r,int *II,int *FI,int *ID,int *FD,char *cad,int I,int F){
	int signo=1, i=I+1,j=0,cont=0;
	char temp[12];
	if(cad[i]!='-' && cad[i]!='+' && !strchr("0123456789",cad[i])) return 0;
	if(cad[i]=='-' || cad[i]=='+'){
		signo=(cad[i]=='-')?-1:1;
		i++;
	}
	while(cad[i]){
		if(cad[i]==' ') break;
		if(strchr("0123456789",cad[i]))
			temp[j++]=cad[i++];
		else return 0;
	}
	if(!cad[i]) return 0;
	temp[j]='\0';
	*r=signo*atoi(temp);
	i++;
	if(cad[i]!='(') return 0;
	*II=i;
	while(cad[i]){
		if(cad[i]=='(') cont++;
		else
			if(cad[i]==')'){
				cont--;
				if(cont==0){
					*FI=i;
					if(cad[i+1]!='(' || cad[F-1]!=')') return 0;
					else{
						*ID=i+1;
						*FD=F-1;
						return 1;
					}
				}
			}
		i++;
	}
	return 0;
}

//-------------------------------------------------------------------------------
// funcion para leer la cadena que esta en los parentesis anidado y verificar si 
// el inicio y el fin estan adyacentes para finalizar
//------------------------------------------------------------------------------

void leerCadena(char *cad,int n){
	int i=0;
	char car;
	while(1){
		car=getchar();
		if(car=='\n') break;
		if(i<n) cad[i++]=car;
	}
	cad[i]='\0';
}


//-----------------------------------------------------------------------------------
// funcion para crear un nodo
//------------------------------------------------------------------------------------
/*
void dibujarNodo(int x,int y){
	SRGP_setColor(6);
	SRGP_fillEllipse(SRGP_defRectangle(x,y,x+60,y+60));
	SRGP_setColor(1);
	SRGP_setLineWidth(3);
	SRGP_ellipse(SRGP_defRectangle(x,y,x+60,y+60));
}
void dibujarLinea(int x, int y, int x1, int y1){
	SRGP_setLineWidth(2);
	SRGP_setColor(1);
	SRGP_lineCoord(315+60/2,); //Linea grande abajo
}
*/
//-----------------------------------------------------------------------------------------
//funcion para dibujar los nodos calculando la posicion del nodo izq y der
//-----------------------------------------------------------------------------------------

void dibujar(Nodo *T, int x,int y,int colorOvalo,int colorLinea,int colorTexto){
	int w,a,d,aux;
	char temp[80];
	sprintf(temp,"555");
	SRGP_inquireTextExtent(temp,&w,&a,&d);
	sprintf(temp,"%d",T->item);
	SRGP_setColor(colorOvalo);
	SRGP_fillEllipse(SRGP_defRectangle(x,y,x+w+8,y+a+8));
	SRGP_setColor(colorTexto);
	SRGP_text(SRGP_defPoint(x+4,y+4),temp);
	if(T->izq!=NULL){
		aux=di(altura(T->izq));
		SRGP_setColor(colorLinea);
		SRGP_lineCoord(x+6,y+6,x-aux+18,y-SV+18);
		dibujar(T->izq,x-aux,y-SV,colorOvalo,colorLinea,colorTexto);
	}
	if(T->der!=NULL){
		aux=di(altura(T->der));
		SRGP_setColor(colorLinea);
		SRGP_lineCoord(x+w-8,y+6,x+w+aux-8,y-SV+8);
		dibujar(T->der,x+aux,y-SV,colorOvalo,colorLinea,colorTexto);
	}
}

//--------------------------------------------------------------------------------------------
// funciones para verificar la altura del arbol
//--------------------------------------------------------------------------------------------
int di(int n){
	return (int)pow(2,n-1)*SH;
}

int altura(Nodo *T){
	return alturaAux(T,1,1);
}

int alturaAux(Nodo *T,int actual,int mayor){
	if(T->izq == T->der)
		return (actual>mayor)?actual:mayor;
	else{
		if(T->izq)
			mayor = alturaAux(T->izq,actual+1,mayor);
		if(T->der)
			mayor = alturaAux(T->der,actual+1,mayor);
		return mayor;
	}
}
