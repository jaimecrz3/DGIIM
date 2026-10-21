// Nombre: Jaime, Apellidos: Corzo Galdó, Titulación: GIM.
// email: jaimecrz04@correo.ugr.es, DNI o pasaporte: 77559045D

#include "latapeones.h"
#include "malla-revol.h"

// ****************************************************************************
//Clase Lata
Lata::Lata(const std::string & nombreArchivoJPG){

    unsigned indice = 1;

    NodoGrafoEscena *lata = new NodoGrafoEscena();

    //Parte central
    NodoGrafoEscena *parte_central = new NodoGrafoEscena();
    //parte_central->ponerNombre("parte central de la lata");
    //parte_central->ponerIdentificador(indice);
    indice++;

    Textura* textura_lata = new Textura(nombreArchivoJPG);
    parte_central->agregar(new Material(textura_lata, 0.75, 0.65, 0.25, 100.0));
    parte_central->agregar(new MallaRevolPLY("lata-pcue.ply",32));

    //Parte superior
    NodoGrafoEscena *parte_superior = new NodoGrafoEscena();
    //parte_superior->ponerNombre("parte superior de la lata");
    //parte_superior->ponerIdentificador(indice);
    indice++;

    parte_superior->agregar(new Material( 0.35, 0.45, 0.25, 100.0));
    parte_superior->agregar(new MallaRevolPLY("lata-psup.ply",32));

    //Parte inferior
    NodoGrafoEscena *parte_inferior = new NodoGrafoEscena();
   // parte_inferior->ponerNombre("parte inferior de la lata");
    //parte_inferior->ponerIdentificador(indice);
    indice++;

    parte_inferior->agregar(new Material(0.35, 0.45, 0.25, 100.0));
    parte_inferior->agregar(new MallaRevolPLY("lata-pinf.ply",32));

    lata->agregar(parte_central);
    lata->agregar(parte_inferior);
    lata->agregar(parte_superior);

    agregar(lata);


}

// ****************************************************************************
//Clase Peon
Peon::Peon(unsigned nPerfiles){
    pm_tras = leerPtrMatriz(agregar(translate(vec3(0.0))));
    agregar(new MallaRevolPLY("peon.ply",nPerfiles));
}

bool Peon::cuandoClick(const glm::vec3 & centro_wc){
    cout << "Click realizado sobre el objeto: '" << leerNombre() << "'" << endl ;

    *pm_tras *= translate(vec3{0.0, 0.0, 1.0});
    return true;
}

// ****************************************************************************
//Clase LataPeones

LataPeones::LataPeones(){

    unsigned identificador_peon_madera = 1;
    unsigned identificador_peon_blanco = 2;
    unsigned identificador_peon_negro = 3;
    unsigned identificador_lata_cocacola = 4;

    ponerNombre("latas y peones");
    NodoGrafoEscena *lata_peones = new NodoGrafoEscena();

    //Peones
    NodoGrafoEscena *peones = new NodoGrafoEscena();
    peones->ponerNombre("peones");

    peones->agregar(translate(vec3{0.0, 0.4, 0.7}));
    peones->agregar(scale(vec3{0.25, 0.25, 0.25}));

    //peon de madera
    Peon *peon_pseudo_especular = new Peon(32);
    peon_pseudo_especular->ponerNombre("Peon de madera");
    peon_pseudo_especular->ponerIdentificador(identificador_peon_madera);

    TexturaXY* texturaXY_madera = new TexturaXY("text-madera.jpg");
    peones->agregar(new Material(texturaXY_madera, 0.2, 0.3, 0.2, 35.0));
    peones->agregar(peon_pseudo_especular);

    //peon blanco
    Peon *peon_blanco = new Peon(32);
    peon_blanco->ponerNombre("Peon blanco");
    peon_blanco->ponerIdentificador(identificador_peon_blanco);

    peones->agregar(translate(vec3{2.5, 0.0, 0.0}));

    //En cada punto p de la superficie de un objeto hay una terna RGB con valores entre 0 y 1,
    //Para cada componente RGB, expresa la fracción de luz reflejada, y por tanto determina el color con el que apreciamos el objeto.
    //
    //El color del objeto afecta únicamente a las componentes ambiental y difusa (no a la componente especular).
    peones->agregar(new Material(0.5, 0.3, 0.2, 5.0));
    peones->agregar(peon_blanco);

    //peon negro
    Peon *peon_negro = new Peon(32);
    peon_negro->ponerNombre("Peon negro");
    peon_negro->ponerIdentificador(identificador_peon_negro);

    peones->agregar(translate(vec3{2.5, 0.0, 0.0}));

    //los objetos aparecerían negros donde no haya iluminación directa.
    //La fraccion e luz reflejada de forma indirecta se mide con el coeficiente ambiental
    peones->agregar(new Material(0.01, 0.2, 0.5, 50.0));
    peones->agregar(peon_negro);

    //Lata
    Lata* lata = new Lata("lata-coke.jpg");
    lata->ponerNombre("Lata de Coca-Cola");
    lata->ponerIdentificador(identificador_lata_cocacola);

    //agregmos la lata y los peones
    lata_peones->agregar(peones);
    lata_peones->agregar(lata);

    agregar(lata_peones);


}

// ****************************************************************************
//Clase VariasLataPeones

VariasLatasPeones::VariasLatasPeones(){

    unsigned identificador_lata_pepsi = 5;
    unsigned identificador_lata_ugr = 6;

    ponerNombre("VariasLatasPeones");
    NodoGrafoEscena *varias_latas_peones = new NodoGrafoEscena();

    NodoGrafoEscena* lata_pepsi = new NodoGrafoEscena();
    lata_pepsi->ponerNombre("Lata de Pepsi");
    lata_pepsi->ponerIdentificador(identificador_lata_pepsi);

    lata_pepsi->agregar(translate(vec3{1.0, 0.0, 0.0}));
    lata_pepsi->agregar(new Lata("lata-pepsi.jpg"));

    NodoGrafoEscena* lata_ugr = new NodoGrafoEscena();
    lata_ugr->ponerNombre("Lata de la UGR");
    lata_ugr->ponerIdentificador(identificador_lata_ugr);

    lata_ugr->agregar(translate(vec3{2.0, 0.0, 0.0}));
    lata_ugr->agregar(new Lata("window-icon.jpg"));

    varias_latas_peones->agregar(lata_pepsi);
    varias_latas_peones->agregar(lata_ugr);

    agregar(varias_latas_peones);


}


