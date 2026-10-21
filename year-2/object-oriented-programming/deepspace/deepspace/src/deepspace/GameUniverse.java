/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Classes/Class.java to edit this template
 */
package deepspace;

import java.util.ArrayList;


public class GameUniverse {
    
    
    //Atributos de la clase
    //Numero de banderas para ganar
    private final static int WIN=10;
    private int currentStationIndex;
    private int turns;
    
    //¿publico/privado?
    private GameStateController gameState;
    private EnemyStarShip currentEnemy;
    private Dice dice;
    
    //No estoy seguro de esto
    private SpaceStation currentStation;
    private ArrayList<SpaceStation> spaceStations; 
    
    //aniadimos practica4
    private boolean haveSpaceCity;
    
    //Metdos de la clase
    
    //Constructor
    //crea el controlador de estados del juego
    //Se inicializa el contador de turnos a 0
    //Se crea el dado
    public GameUniverse(){
        
        gameState = new GameStateController();
        turns = 0;
        dice = new Dice();
        currentStationIndex = -1;
        currentEnemy = null;
        currentStation = null;
        spaceStations = new ArrayList<>();
        haveSpaceCity = false;
        
    }
    /*
    Se realiza un combate entre la
    estación espacial y el enemigo que se reciben como parámetros. Se sigue el procedimiento descrito
    en las reglas del juego: sorteo de quién dispara primero, posibilidad de escapar, asignación del
    botín, anotación del daño pendiente, etc. Se devuelve el resultado del combate.
    */
    CombatResult combat(SpaceStation station,  EnemyStarShip enemy){
        
        
        GameCharacter ch= dice.firstShot();
        boolean enemyWins; 
        float fire;
        ShotResult result;
        
        CombatResult combatResult;
        
        if (ch==GameCharacter.ENEMYSTARSHIP){
            
            fire=enemy.fire();
            result= station.receiveShot(fire);
            
            if (result==ShotResult.RESIST){
                
                fire=station.fire();
                result= enemy.receiveShot(fire);
                
                enemyWins=(result==ShotResult.RESIST);
            }
            
            else enemyWins=true;
 
        }
        
        else {
            
            fire=station.fire();
            result= enemy.receiveShot(fire);
            enemyWins=(result==ShotResult.RESIST);
            
        }
            
        if (enemyWins){
            
            float s = station.getSpeed();
            boolean moves=dice.spaceStationMoves(s);
            
            if(!moves){
                Damage damage = enemy.getDamage();
                station.setPendingDamage(damage);
                combatResult=CombatResult.ENEMYWINS;
            }
            
            else{
                
                station.move();
                combatResult=CombatResult.STATIONESCAPES;
            }
        }
        
        else{
            
            Loot aLoot = enemy.getLoot();
            /*
                Cuando la estación espacial vence al enemigo, al recibir el botín hay que añadir el código necesario
    para tratar la conversión a estación eficiente o a ciudad espacial si ese botín así lo indica. En este
    código se utilizará los dos nuevos métodos explicados anteriormente.
            */
            Transformation transformation = station.setLoot(aLoot);
            if(transformation==Transformation.GETEFFICIENT){
                combatResult = CombatResult.STATIONWINSANDCONVERTS;
                makeStationEfficient();
            } else if(transformation==Transformation.SPACECITY){
                combatResult = CombatResult.STATIONWINSANDCONVERTS;
                createSpaceCity();
            }else{
                combatResult=CombatResult.STATIONWINS;
            }
            
        }
        
        gameState.next(turns, spaceStations.size());
        
        return combatResult;
        
    }
    
    
    /*
    Si la aplicación se encuentra en un estado en donde el combatir está
    permitido, se realiza un combate entre la estación espacial que tiene el turno y el enemigo actual. Se
    devuelve el resultado del combate.
    */

    public CombatResult combat(){
        
        CombatResult output=CombatResult.NOCOMBAT;
        GameState state =gameState.getState();
        
        
        if ((state==GameState.BEFORECOMBAT) || state==GameState.INIT)
            
           output= combat(this.currentStation,this.currentEnemy);
           
        
        
      
        return output;
    }

    /*
    Los siguientes metodos delegan en el método homónimo de la estación espacial que tenga
    el turno siempre que se cumpla que el estado del juego sea INIT o 
    AFTERCOMBAT. En caso contrario no tienen ningún efecto.
    */
    
    public void discardHangar(){
        
        if(gameState.getState()==GameState.INIT || gameState.getState()==GameState.AFTERCOMBAT){
            
            currentStation.discardHangar();
            
        }
        
    }
    
    
    /*
    Se intenta descartar el potenciador de escudo con índice i de la
    colección de potenciadores de escudo en uso. Además de perder el potenciador de escudo, se debe
    actualizar el daño pendiente (pendingDamage) si es que se tiene alguno.
    */

    public void discardShieldBooster(int i){
        
        if(gameState.getState()==GameState.INIT || gameState.getState()==GameState.AFTERCOMBAT){
            
            currentStation.discardShieldBooster(i);
            
        }
        
    }

    public void discardShieldBoosterInHangar( int i){
        
        if(gameState.getState()==GameState.INIT || gameState.getState()==GameState.AFTERCOMBAT){
            
            currentStation.discardShieldBoosterInHangar(i);
            
        }
        
    }
    
    /*
    Se intenta descartar el arma con índice i de la colección de armas en uso.
    Además de perder el arma, se debe actualizar el daño pendiente (pendingDamage) si es que se tiene
    alguno.
    */

    public void discardWeapon(int i){
        
        if(gameState.getState()==GameState.INIT || gameState.getState()==GameState.AFTERCOMBAT){
            
            currentStation.discardWeapon(i);
            
        }
        
    }

    public void discardWeaponInHangar(int i){
        
        if(gameState.getState()==GameState.INIT || gameState.getState()==GameState.AFTERCOMBAT){
            
            currentStation.discardWeaponInHangar(i);
            
        }
        
    }
    
    public void mountShieldBooster(int i){
        
        if(gameState.getState()==GameState.INIT || gameState.getState()==GameState.AFTERCOMBAT){
            
            currentStation.mountShieldBooster(i);
            
        }
        
    } 

    public void mountWeapon(int i){
        
        if(gameState.getState()==GameState.INIT || gameState.getState()==GameState.AFTERCOMBAT){
            
            currentStation.mountWeapon(i);
            
        }
        
    }

    //Getters
    public GameState getState(){ return (gameState.getState());}
    

    //Interfaz
    public GameUniverseToUI getUIversion(){return (new GameUniverseToUI(currentStation, currentEnemy));}

    //Devuelve true si la estación espacial que tiene el turno ha llegado al
    //número de medallas necesarias para ganar.
    public boolean haveAWinner(){
        
        return(currentStation.getNMedals()>=WIN);
        
    }

    /*
    Este método inicia una partida. Recibe una colección con los
    nombres de los jugadores. Para cada jugador, se crea una estación espacial y se equipa con
    suministros, hangares, armas y potenciadores de escudos tomados de los mazos de cartas
    correspondientes. Se sortea qué jugador comienza la partida, se establece el primer enemigo y
    comienza el primer turno.
    */
    public void init(ArrayList<String> names){
        
        if(gameState.getState() == GameState.CANNOTPLAY){
            
            CardDealer dealer = CardDealer.getInstance();
            //En el diagrama viene con i=1 pero creo que es asi
            for(int i=0; i < names.size(); i++){
                
                SuppliesPackage sup = dealer.nextSuppliesPackage();
                SpaceStation station = new SpaceStation(names.get(i),sup);
                
                //Añadimos la estación que hemos creado a spaceStations
                spaceStations.add(station);
                
                int nh = dice.initWithNHangars();
                int nw = dice.initWithNWeapons();
                int ns = dice.initWithNShields();
                
                //Creamos Loot
                Loot lo = new Loot(0, nw, ns, nh, 0);
                
                //Lo añadimos a la estación
                station.setLoot(lo);
                
            }
            
            currentStationIndex= dice.whoStarts(names.size());
            currentStation = spaceStations.get(currentStationIndex);
            currentEnemy = dealer.nextEnemy();
            
            gameState.next(turns, spaceStations.size());
      
        }
        
    }

    /*
        Se comprueba que el jugador actual no tiene ningún daño pendiente de cumplir,
    en cuyo caso se realiza un cambio de turno al siguiente jugador con un nuevo enemigo con quien
    combatir, devolviendo true. Se devuelve false en otro caso.
    */
    public boolean nextTurn(){
        
        boolean output = false;
        
        if(gameState.getState() == GameState.AFTERCOMBAT){
            
            if(currentStation.validState()){
                
                currentStationIndex = (currentStationIndex + 1)%spaceStations.size();
                turns++;
                
                currentStation = spaceStations.get(currentStationIndex);
                currentStation.cleanUpMountedItems();
                
                CardDealer dealer = CardDealer.getInstance();
                
                currentEnemy = dealer.nextEnemy();
                
                gameState.next(turns, spaceStations.size());
                
                output = true;
                
            }
            
        }
        
        return (output);
        
    }
    
    //--------------------------------------------
    //Método Practica4
    
    
    /*
        Si el juego no dispone ya de una ciudad espacial (haveSpaceCity==false)
    convierte la estación espacial actual en una ciudad espacial usando como estación espacial base la
    actual y como colaboradoras el resto las estaciones espaciales. Actualiza la referencia a
    currentStation y también el atributo haveSpaceCity.
    */
    private void createSpaceCity(){
        
        if(!haveSpaceCity){
            
            currentStation = new SpaceCity(currentStation, spaceStations);
            spaceStations.set(currentStationIndex, currentStation);
            
            haveSpaceCity = true;
            
        }
        
    }
    
    /*
        Pregunta al dado si debe convertir la estación espacial actual en una
    estación eficiente o en una estación eficiente beta. 
    El dado es el que determina la conversión en un tipo o en otro con el 
    método extraEfficiency(). Actualiza la referencia a currentStation.
    */
    private void makeStationEfficient(){
        
        currentStation = new PowerEfficientSpaceStation(currentStation);
        if(dice.extraEfficiency()){
            currentStation = new BetaPowerEfficientSpaceStation(currentStation);
        }
        spaceStations.set(currentStationIndex, currentStation);
        
    }
    
    //-----------------------------------------------
    
    public String toString(){
        
        String spaceStations_string= "spaceStations: ";
        for(int i=0; i<spaceStations.size(); i++){
            spaceStations_string += spaceStations.get(i).toString() + " ";
        }
        
        String output= "currentStationIndex: "    + currentStationIndex + "\n" +
                       "turns: "        +turns           + "\n" +
                        "gameState: "     + gameState.toString()  + "\n" +
                        "currentEnemy: " + currentEnemy.toString()      + "\n" +
                        "currentStation: "        + currentStation.toString()  + "\n" +
                        spaceStations_string + "\n";
        
        return output;
    }
    
}