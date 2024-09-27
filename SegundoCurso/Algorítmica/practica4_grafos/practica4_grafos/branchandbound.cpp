#include <iostream>
#include <vector>
#include <queue>
#include <random>
#include <chrono>
#include <algorithm>
#include <fstream>

using namespace std;
using namespace std::chrono;


bool orden_decreciente(int i, int j){
    return i>j;
}

class Solucion{
private:
    const int NULO = -1;
    const int END = -2;
    vector<int> solucion; // Almacenamos la solucion
    int pos_e;  // Posicion de la ultima decision en solucion
    int conveniencia_acumulada; // Conveniencia acumulada
    int invitados_sentados; // Numero de invitados sentados
    int estimador_; // Valor del estimador para el nodo X
    int num_invitados;
    vector<vector<int>> datos;
    vector<bool> estan_sentados;
    vector<int> nodos;
    int num_cota;

public:
    Solucion(const vector<vector<int>> &m, int cota){
        num_cota = cota;
        num_invitados = m.size();
        datos = m;
        for(int i=0; i<num_invitados; i++){
            solucion.push_back(NULO);
        }

        conveniencia_acumulada=0;
        invitados_sentados=0;
        pos_e=-1;
        estimador_=0;

        for(int i = 0; i < num_invitados; i++){
            estan_sentados.push_back(false);
        }

        for(int i = 0; i <num_invitados; i++){
            for(int j = 0; j < num_invitados; j++){
                if(i != j){ //Los de la diagonal no nos interesan
                    nodos.push_back(datos.at(i).at(j));
                }
            }
        }

        sort(nodos.begin(), nodos.end(), orden_decreciente);
        PrimerValorComp(0);
    }

    bool Factible() const {

        bool es_factible = true;
        int i = 0;

        while (i <pos_e && es_factible){
            if(solucion.at(i) == solucion.at(pos_e)) es_factible = false;
            else i++;
        }

        return es_factible;

    }

    vector<int> getSolucion() const{
        return solucion;
    }

    void PrimerValorComp(int k){
        solucion.at(k) = 0;
        estan_sentados.at(solucion.at(k)) = true;
        pos_e++; // Cada vez que se inicializa un valor se aumneta la posición, ya que pos_e es la posicion de la ultima
                        // decision en solucion, es decir, del ultimo elemento que hemos añadido a solucion
    }

    void SigValComp(int k){
        int val = solucion.at(k);



        if (val < num_invitados -1) {
            if(val == NULO){
                invitados_sentados++;
            }

            //if(val == NULO){ // POS_E NO SE UTILIZA PARA ESO, TAL Y COMO ESTA AHORA ESTA HACIENDO LA FUCNION DE CONTTADOR Y NO ES ESO
               // pos_e++;
                //estan_sentados.at(k)=true;
            //}
            val++;

            solucion.at(k)=val;
           // estan_sentados.at(solucion.at(k)) = true;
            estimador_ =DecisionesTomadas()+CotaLocal(num_cota);
        }
        else {
            solucion.at(k) = END;
        }

    }

    int Comp(int k) const{
        return solucion.at(k);
        //estan_sentados.at(solucion.at(k)) = false;
    }


    //Para el calculo de las cotas locales vamos a usar el algoritmo greedy
    /**
      * Calcula la cota local asociada a un nodo; es decir la mejor solucion que se podría obtener al exapandir un nodo
      * @return DEvuelve la cota local de ese nodo
      */
    int CotaLocal(int cota) const{
        int cota_local =0;
        if(cota==1){
            cota_local += Estimador1();
        }else if(cota==2){
            cota_local += Estimador2();
        }else{
            cota_local += Estimador3();
        }

        return cota_local;
    }
    int DecisionesTomadas() {
        return Evalua();
    }
    int Estimador1() const{

        int invitados_sin_sentar= num_invitados - (pos_e+1);
        int estimador=0;



            //Cogemos el que tenga mayor conveniencia con el(es decir, de su fila en la matriz datos)
            bool seguir=true;
            int conveniencia_=0;
            int i=0;

           conveniencia_ = nodos.front();


            /*while(i < nodos.size() && seguir){

                //Cuando encontremos el primer nodo con id1=ultimo invitado sentado paramos(los nodos estan ordenados de
                //mayor a menor conveniencia)
                if(nodos[i].id1 == solucion.at(pos_e)  && estan_sentados.at(nodos[i].id2)==false){

                    conveniencia_ = nodos[i].conveniencia;
                    seguir = false;

                }else{
                    i++;
                }

            }*/
        estimador = conveniencia_;

        return  Evalua2(this->getSolucion(), pos_e+1)+(2*estimador*invitados_sin_sentar);

    }

    int Estimador2() const{
        int estimador2 = 0;

        for(int i = 0; i < datos.size(); i++){

            if(!estan_sentados.at(i)) {
                vector<int>::const_iterator it = max_element(datos.at(i).begin(), datos.at(i).end());
                estimador2+=2*(*it);

            }
        }
        return  Evalua2(this->getSolucion(), pos_e+1)+estimador2;
    }


    int Estimador3() const{
        int estimador3 = 0;
        int max = 0;

        for(int i = 0; i < datos.size(); i++){
            max= 0;
            if(!estan_sentados.at(i)) {
                int indice_max;
                for(int j = 0; j < datos.at(i).size(); j++){

                    if(datos.at(i).at(j) > max )
                        max = datos.at(i).at(j);
                    indice_max=j;
                }

                //BUSCAR EL OTRO MÁXIMO
                int otro_max = 0;
                for(int j = 0; j < datos.at(i).size(); j++){

                    if(datos.at(i).at(j) > otro_max && indice_max!=j)
                        otro_max = datos.at(i).at(j);
                }

                max +=otro_max;

            }

            estimador3+=max;
        }

        return  Evalua2(this->getSolucion(), pos_e+1)+estimador3;
    }

    bool HayMasValores (int k) const{

        return ((solucion.at(k) <= (num_invitados-1)&& solucion.at(k)>=-1));

    }

    int Evalua() { //es como el consigue suma

        int suma = Evalua2(getSolucion(), pos_e+1);
/*
        int suma = 0;
        int tam = pos_e+1;
        if(tam > 1) {


            for (int i = 0; i < tam; i++) {

                int val = getSolucion().at(i);

                if (i == 0) {

                    suma += datos.at(val).at(getSolucion().at(i + 1));
                    suma += datos.at(val).at(getSolucion()[tam - 1]);

                } else if (i < (tam - 1)) {
                    suma += datos.at(val).at(getSolucion().at(i + 1));
                    suma += datos.at(val).at(getSolucion().at(i - 1));

                } else {

                    suma += datos.at(val).at(getSolucion()[0]);
                    suma += datos.at(val).at(getSolucion().at(i - 1));

                }
            }
        }

*/
        return suma;

    }

    int Evalua2(const vector<int> &v, int tam) const{
        int suma = 0;
        if(tam > 1) {
            for (int i = 0; i < tam; i++) {

                int val = v.at(i);

                if (i == 0) {

                    suma += datos.at(val).at(v.at(i + 1));
                    suma += datos.at(val).at(v[tam - 1]);

                } else if (i < (tam - 1)) {
                    suma += datos.at(val).at(v.at(i + 1));
                    suma += datos.at(val).at(v.at(i - 1));

                } else {

                    suma += datos.at(val).at(v[0]);
                    suma += datos.at(val).at(v.at(i - 1));

                }
            }
        }

        return suma;
    }

    int NumComponentes() const{

        return solucion.size();

    }

    bool EsSolucion() const{

        return (pos_e == num_invitados-1)&& this->Factible();

    }

    int CompActual() const{
        return pos_e;
    }

    bool operator<(const Solucion &s) const{
        return (estimador_<s.estimador_);
    }

    Solucion& operator=(const Solucion &s) {
        this->solucion = s.solucion;
        this->pos_e = s.pos_e;
        this->conveniencia_acumulada = s.conveniencia_acumulada;
        this->invitados_sentados = s.invitados_sentados;
        this->estimador_ = s.estimador_;
        this->num_invitados = s.num_invitados;

        return *this;
    }

    /**
     * Devuelve el numerode comensales que ya hay sentados en la mesa
     * @return Tamaño del vector solucion
     */
    int size() const{
        return num_invitados;
    }


    int Greedy() {
        vector<int> sol;
        int suma = 0;
        vector<bool> usados;
        int tam = size();


        for (int i = 0; i < tam; i++) {
            usados.push_back(false);
        }

        int invitado_actual = 0;
        sol.push_back(0);
        usados.at(0) = true;
        while (sol.size() < tam) {

            int max = -1;
            int pos_max = invitado_actual;

            for (int i = 0; i < tam; ++i) {
                int c = datos.at(invitado_actual).at(i);
                if (c > max && c>0 && !usados.at(i)) {
                    max = c;
                    pos_max = i;
                }
            }

            usados.at(pos_max) = true;
            sol.push_back(pos_max);
            invitado_actual = pos_max;
        }

        suma = Evalua2(sol, sol.size());

        solucion = sol;
        pos_e = num_invitados-1;
        conveniencia_acumulada = suma;
        invitados_sentados = num_invitados;
        estimador_ = suma;
        estan_sentados = usados;
        /*
        for(int x=0; x<sol.size();x++){
            cout << sol.at(x) << " ";
        }
        cout << ". TOTAL: " << suma << endl;
        */

        return suma;
    }




};



Solucion Branch_and_Bound( const vector<vector<int>> &datos, int num_cota){
    priority_queue<Solucion> Q;
    Solucion n_e(datos, num_cota), mejor_solucion(datos, num_cota);
    int k;
    int cota_global=mejor_solucion.Greedy();
    //int cota_global = -1;
    int conveniencia_actual;

    Q.push(n_e);

    while(!Q.empty() && ( Q.top().CotaLocal(num_cota) > cota_global )){
        n_e=Q.top();
        Q.pop();
        k= n_e.CompActual();
        for(n_e.PrimerValorComp(k+1); n_e.HayMasValores(k+1); n_e.SigValComp(k+1)){
            if(n_e.EsSolucion()){
                conveniencia_actual=n_e.Evalua();
                if(conveniencia_actual > cota_global){
                    cota_global=conveniencia_actual;
                    mejor_solucion=n_e;
                }
            }else{
                int cota = n_e.CotaLocal(num_cota);
                if(cota>cota_global && n_e.Factible() ){
                    Q.push(n_e);
                }
            }

        }
    }

    return mejor_solucion;
}


int main(int argc, char *argv[]) {
    /*
    if (argc != 2 ){
        cerr << "Formato " << argv[0] << " <fichero_conveniencias>" << endl;
        return -1;
    }

    char *fich = argv[1];

    ifstream fin;
    fin.open(fich);

    int cota;
    int num_invitados;
    vector<vector<int>> datos;

    // Lectura de la matriz de aristas
    if(fin) {
        fin >> cota;
        fin >> num_invitados;

        vector<int> aux;
        for (int i = 0; i < num_invitados; i++) {
            for (int j = 0; j < num_invitados; j++) {
                int conveniencia;
                fin >> conveniencia;
                if (fin) {
                    aux.push_back(conveniencia);
                }
            }
            datos.push_back(aux);
            aux.clear();
        }
    }
    else{
        cout << "ERROR: no se puedo abrir el fichero " << argv[1] << endl;
        return -1;
    }
     */


    if (argc != 2){
        cerr << "Formato " << argv[0] << " <Cota a usar: {1,2,3}>" << endl;
        return -1;
    }

    int cota = atoi(argv[1]);


    vector<vector<int>> datos;

    const int MAX = 1001;
    int num_invitados = 5;
    vector<int> aux;
    for(int i=0; i<num_invitados; i++){
        aux.push_back(0);
    }
    for(int i=0; i<num_invitados; i++){
        datos.push_back(aux);
    }

    //srand(time(NULL));

    for(int i = 0; i < num_invitados; i++){
        for(int j = 0; j < num_invitados; j++){
            if(i==j) datos.at(i).at(j) = 0;
            else datos.at(i).at(j)=rand()%MAX;

        }
    }



    for(int i = 0; i < num_invitados; i++){
        for(int j = 0; j < num_invitados; j++){
            cout << datos.at(i).at(j) << "   " ;
        }
        cout << endl;
    }
    cout << endl;

// Se llama al algoritmo
    high_resolution_clock::time_point t_antes,t_despues;
    duration<double> transcurrido;

    t_antes = high_resolution_clock::now();
    Solucion sol = Branch_and_Bound(datos, cota);
    t_despues = high_resolution_clock::now();

    transcurrido = duration_cast<duration<double>> (t_despues - t_antes);


    cout << endl << " SOLUCION "  << endl;
    for(int i = 0; i < sol.size(); i++){
        cout << sol.getSolucion().at(i) << "   " ;
    }

    cout << "Suma: " << sol.Evalua() << "    ";

    cout << "Cota: " << cota << endl;

    cout << "Tiempo: " << transcurrido.count() << endl;
    return 0;
}
