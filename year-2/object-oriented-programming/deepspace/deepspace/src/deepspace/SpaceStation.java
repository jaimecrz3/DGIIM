/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Classes/Class.java to edit this template
 */
package deepspace;

import java.util.ArrayList;

//Representa una nave espacial con todos sus elementos.
public class SpaceStation implements SpaceFighter{
    
    //Maximo de unidades combustible que puede tener una estacion.
    private final static int MAXFUEL = 100;
    
    //Unidades de escudo perdidas por cada unidad de potencia de disparo recibido.
    private final static float SHIELDLOSSPERUNITSHOT = 0.1f;
    
    //Poder de disparo.
    private float ammoPower;
    
    //Cantidad de combustible.
    private float fuelUnits;
    
    //Nombre.
    private String name;
    
    //Numero de medallas.
    private int nMedals;
    
    //Poder de escudo.
    private float shieldPower;
    
    
    //Lista de armas que puede tener una estacion.
    private ArrayList<Weapon> weapons;
    
    //Lista de escudos que puede tener una estacion.
    private ArrayList <ShieldBooster> shieldBoosters;
    
    //Numero de hangares (0,1)
    private Hangar hangar;
    
    //Daño (0,1)
    private Damage pendingDamage;
    
    

    SpaceStation(String n, SuppliesPackage supplies){

        name=n;
        ammoPower=0.0f;
        fuelUnits=0.0f;
        shieldPower=0.0f;
        weapons=new ArrayList<>();
        shieldBoosters=new ArrayList<>();
        hangar=null;
        pendingDamage=null;
        nMedals=0;
        
        receiveSupplies(supplies);

    }
    
    public SpaceStation(SpaceStation station){
        
        this(station.name,new SuppliesPackage(station.ammoPower,station.fuelUnits,station.shieldPower));
        
        this.weapons= new ArrayList<>(station.weapons);
        this.shieldBoosters= new ArrayList<>(station.shieldBoosters);
        
        if (station.hangar!= null) this.hangar= new Hangar (station.hangar);
        
        if (station.pendingDamage!=null) this.pendingDamage= station.pendingDamage.copy();
        
        this.nMedals=station.nMedals;
    }
    
    
    /*
    Fija la cantidad de combustible al valor pasado como parámetro sin
    que nunca se exceda del límite.
    */
    
    private void assignFuelValue(float f){
        
        if(f>MAXFUEL) f=MAXFUEL;
        
        fuelUnits=f;
    }
    
    
    //Si pendingDamage no tiene efecto lo fija a null.
    private void cleanPendingDamage(){if(pendingDamage.hasNoEffect()) pendingDamage=null;}
    
    
    
    /*
    Eliminar todas las armas y los potenciadores de escudo montados a las que no 
    les queden usos.
    */
    public void cleanUpMountedItems(){
        
        //Lo recorremos al reves para no acceder a posiciones que no existen
        for (int i=weapons.size()-1; i>=0; i--) 
            if (weapons.get(i).getUses()==0) weapons.remove(i);
        
        for (int i=shieldBoosters.size()-1; i>=0; i--) 
            if (shieldBoosters.get(i).getUses()==0) shieldBoosters.remove(i);
    }
    
    //Fija hangar a null para indicar que no se dispone de él.
    
    public void discardHangar(){hangar=null;}
    
    
    
    /*
    Se intenta descartar el potenciador de escudo con índice i de la
    colección de potenciadores de escudo en uso. Además de perder el potenciador de escudo, se debe
    actualizar el daño pendiente (pendingDamage) si es que se tiene alguno.
    */
    
    
    public void discardShieldBooster(int i){
        
        if(i >= 0 && i < shieldBoosters.size()){
            
            shieldBoosters.remove(i);
            
            if(pendingDamage != null){
                
                pendingDamage.discardShieldBooster();
                
                this.cleanPendingDamage();
                
            }
            
        }
        
    }
    
    
    /*
    Si se dispone de hangar, se solicita al mismo descartar el potenciador de 
    escudo con índice i.
    */
    
    public void discardShieldBoosterInHangar(int i){if (hangar!=null) hangar.removeShieldBooster(i);}
    
    
    /*
        Se intenta descartar el arma con índice i de la colección de armas en uso.
    Además de perder el arma, se debe actualizar el daño pendiente (pendingDamage) si es que se tiene
    alguno.
    */
    
    public void discardWeapon(int i){
        
        if(i >= 0 && i < weapons.size()){
            
            Weapon w = new Weapon(weapons.remove(i));
            
            if(pendingDamage != null){
                
                pendingDamage.discardWeapon(w);
                
                this.cleanPendingDamage();
                
            }
            
        }
        
    }
    
    
    /*
    Si se dispone de hangar, se solicita al mismo descartar el arma con índice i.
    */
    
    public void discardWeaponInHangar(int i){if (hangar!=null) hangar.removeWeapon(i);}
    
    
    
    //Getters
    
    /*
    Realiza un disparo y se devuelve la energía o potencia del mismo. Para ello 
    se multiplica la potencia de disparo por los factores potenciadores 
    proporcionados por todas las armas.
    */
    //Implementamos el método de la interfaz
    @Override
    public float fire(){
        
        float factor=1.0f;
        
        for (int i=0; i<weapons.size(); i++) factor*= weapons.get(i).useIt();
    
        return ammoPower*factor;
    }
    
    
    /*
    Se usa el escudo de protección y se devuelve la energía del mismo. Para ello
    se multiplica la potencia del escudo por los factores potenciadores 
    proporcionados por todos los potenciadores de escudos de los que se dispone.
    */
    //Implementamos el método de la interfaz
    @Override
    public float protection(){
        
        float factor=1.0f;
        
        for (int i=0; i<shieldBoosters.size(); i++) factor*= shieldBoosters.get(i).useIt();
    
        return shieldPower*factor;
    
    }
    
    
    public float getAmmoPower(){return ammoPower;}
    
    public float getFuelUnits(){return fuelUnits;}
    
    public Hangar getHangar(){return hangar;}
    
    public String getName(){return name;}
    
    public int getNMedals(){return nMedals;}
    
    public Damage getPendingDamage(){return pendingDamage;}
    
    public ArrayList<ShieldBooster> getShieldBoosters(){return shieldBoosters;}
    
    public float getShieldPower(){return shieldPower;}
    
    
    /*
    Devuelve la velocidad de la estacion, que se calcula como la fracción entre 
    las unidades de combustible de las que dispone en la actualidad la estación 
    espacial respecto al máximo unidades de combustible que es posible almacenar.
    */
    
    public float getSpeed(){return (fuelUnits/MAXFUEL);}
    
    
    //Interfaz
    public SpaceStationToUI getUIversion(){return (new SpaceStationToUI(this));}
    
    public ArrayList<Weapon> getWeapons(){return weapons;}
    
    
    //Setters
    
    
    /*
    Se intenta montar el potenciador de escudo con el índice i dentro del hangar. 
    Si se dispone de hangar, se le indica que elimine el arma de esa posición y 
    si esta operación tiene éxito (el hangar proporciona el potenciador), se 
    añade el potenciador a la colección de armas en uso.
    */
    
    public void mountShieldBooster(int i){
        
        if (hangar!=null){
            
            ShieldBooster shieldbooster=hangar.removeShieldBooster(i);
            if (shieldbooster!=null) shieldBoosters.add(shieldbooster);
        }
    }
    
    
    /*
    Se intenta montar el arma con el índice i dentro del hangar. Si se dispone
    de hangar, se le indica que elimine el arma de esa posición y si esta 
    operación tiene éxito (el hangar proporciona el arma), se añade el arma a la 
    colección de armas en uso.
    */
    
    public void mountWeapon(int i){
    
         if (hangar!=null){
            
            Weapon weapon=hangar.removeWeapon(i);
            if (weapon!=null) weapons.add(weapon);
        }
    }
    
    
    /*
    Decremento de unidades de combustible disponibles a causa de un desplazamiento. 
    Al número de las unidades almacenadas se les resta una fracción de las 
    mismas que es igual a la velocidad de la estación. Las unidades de combustible 
    no pueden ser inferiores a 0.
    */
    
    public void move(){
        
        fuelUnits-= getSpeed();
        
        if(fuelUnits<0) fuelUnits=0;
    }
    
    
    
    /*
    Si no se dispone de hangar, el parámetro pasa a ser el hangar de la
    estación espacial. Si ya se dispone de hangar esta operación no tiene efecto.
    */
    
    public void receiveHangar(Hangar h){if(hangar==null) hangar=h;}
    
    
    /*
    Si se dispone de hangar, devuelve el resultado de intentar
    añadir el potenciador de escudo al mismo. En caso contrario false.
    */
    
    public boolean receiveShieldBooster(ShieldBooster s){
    
        boolean received=false;
        
        if (hangar!=null) received=hangar.addShieldBooster(s);
        
        return received;
    }
    
    
    /*
    Realiza las operaciones relacionadas con la recepción del impacto de un 
    disparo enemigo. Ello implica decrementar la potencia del escudo en función 
    de la energía del disparo recibido como parámetro y devolver el resultado de
    si se ha resistido el disparo o no.
    */
    //Implementamos el método de la interfaz
    @Override
    public ShotResult receiveShot(float shot){
        
        ShotResult output= ShotResult.DONOTRESIST;
    
        if (protection()>=shot){
            
            shieldPower-=SHIELDLOSSPERUNITSHOT*shot;
            if (shieldPower < 0) shieldPower=0.0f;
            
            output = ShotResult.RESIST;
            
        }
        
        else shieldPower=0.0f;
        
        return output;
    }
    
    
    /*
    La potencia de disparo, la del escudo y las unidades de
    combustible se incrementan con el contenido del paquete de suministro.
    */
    
    public void receiveSupplies(SuppliesPackage s){
    
        ammoPower+=s.getAmmoPower();
        shieldPower+=s.getShieldPower();
        assignFuelValue(fuelUnits + s.getFuelUnits());
    }
    
    
    /*
    Si se dispone de hangar, devuelve el resultado de intentar
    añadir el arma al mismo. En caso contrario false.
    */
    
    public boolean receiveWeapon(Weapon w){
        
        boolean received=false;
        
        if (hangar!=null) received=hangar.addWeapon(w);
        
        return received;
    }
    
    /*
        Recepción de un botín. Por cada elemento que indique el botín (pasado
    como parámetro) se le pide a CardDealer un elemento de ese tipo y se intenta almacenar con el
    método receive*() correspondiente. Para las medallas, simplemente se incrementa su número según
    lo que indique el botín.
    */
    
    public Transformation setLoot(Loot loot){
    
        CardDealer dealer = CardDealer.getInstance();
        
        if(loot.getNHangars()>0){
            
            Hangar hangar_ = dealer.nextHangar();
            this.receiveHangar(hangar_);
            
        }
        
        for(int i=0; i < loot.getNSupplies(); i++){
            
            SuppliesPackage sup = dealer.nextSuppliesPackage();
            this.receiveSupplies(sup);
            
        }
        
        for(int i=0; i < loot.getNWeapons(); i++){
            
            Weapon weap = dealer.nextWeapon();
            this.receiveWeapon(weap);
            
        }
        
        for(int i=0; i < loot.getNShields(); i++){
            
            ShieldBooster sh = dealer.nextShieldBooster();
            this.receiveShieldBooster(sh);
            
        }
        
        nMedals += loot.getNMedals();
        
        
        Transformation transformation=Transformation.NOTRANSFORM;
        
        if (loot.getEfficient()) transformation=Transformation.GETEFFICIENT;
            
        else if (loot.spaceCity()) transformation=Transformation.SPACECITY;
        
        return transformation;
    
    }
    
    
    
    /*
    Se calcula el parámetro ajustado (adjust) a la lista de armas y
    potenciadores de escudo de la estación y se almacena el resultado en el 
    atributo correspondiente.
    */
    public void setPendingDamage(Damage d){pendingDamage=d.adjust(weapons, shieldBoosters);}
    
    
    /*
    Devuelve true si la estación espacial está en un estado válido. Eso implica 
    que o bien no se tiene ningún daño pendiente o que este no tiene efecto.
    */
    
    public boolean validState(){return (pendingDamage==null || pendingDamage.hasNoEffect());}
    
    public String toString(){
        
        String weapons_string= "Weapons: ";
        for(int i=0; i<weapons.size(); i++){
            weapons_string += weapons.get(i).toString() + " ";
        }
        
        String shieldBoosters_string= "shieldBoosters: ";
        for(int i=0; i<shieldBoosters.size(); i++){
            weapons_string += shieldBoosters.get(i).toString() + " ";
        }
        
        
        
        String output= "ammoPower: "        + ammoPower           + "\n" +
                       "fuelUnits: "        + fuelUnits           + "\n" +
                        "name: "            + name          + "\n" +
                        "nMedals: "        + nMedals           + "\n" +
                        "shieldPower: "        + shieldPower          + "\n" +
                        weapons_string + "\n" +
                        shieldBoosters_string + "\n" +
                        hangar.toString() + pendingDamage.toString();
        
        return output;
    }
    
   
}
 
