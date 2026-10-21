#encoding: UTF-8
require_relative 'ShieldBooster'
require_relative 'GameUniverseToUI'
require_relative 'ShieldBooster'
require_relative 'Weapon'
require_relative 'Dice'
require_relative 'GameStateController'
require_relative 'GameState'
require_relative 'CardDealer'
require_relative 'SpaceStation'
require_relative 'CombatResult'
require_relative 'SpaceCity'
require_relative 'PowerEfficientSpaceStation'
require_relative 'BetaPowerEfficientSpaceStation'
require_relative 'Transformation'

module Deepspace

    class GameUniverse
        
        #Numero de banderas para ganar
        #Es de clase
        @@WIN = 10

        #Estado de inactividad para indices.
        @@NOUSE=-1

        #Metodos de la clase
        
        #Constructor
        #crea el controlador de estados del juego
        #Se inicializa el contador de turnos a 0
        #Se crea el dado
        def initialize
            
            @gameState=GameStateController.new()
            @turns = 0
            @dice = Dice.new()
            @spaceStations = Array.new
            @currentStationIndex =  @@NOUSE
            @currentEnemy = nil    #Por defecto de inicializa a nil pero para que se vea
            @currentStation = nil
            @haveSpaceCity = false
            
        end
        


        #Se realiza un combate entre la estación espacial y el enemigo que se reciben 
        #como parámetros. Se sigue el procedimiento descrito en las reglas del juego: 
        #sorteo de quién dispara primero, posibilidad de escapar, asignación del botín, 
        #anotación del daño pendiente, etc. Se devuelve el resultado del combate.
        
        #Le cambiamos el nombre porque en ruby no se permite sobrecarga
        
        def combatGo(station, enemy)
            ch= @dice.firstShot()
            
            
            if (ch==GameCharacter::ENEMYSTARSHIP)
                
                fire=enemy.fire()
                result= station.receiveShot(fire)
                
                if (result==ShotResult::RESIST)
                    
                    fire=station.fire()
                    result= enemy.receiveShot(fire)
                    
                    enemyWins=(result==ShotResult::RESIST)

                else enemyWins=true
                
                end
            
            
            else 
                
                fire=station.fire()
                result= enemy.receiveShot(fire)
                enemyWins=(result==ShotResult::RESIST)
                
            end
                
            if (enemyWins)
                
                s = station.speed()
                moves=@dice.spaceStationMoves(s)
                
                if(!moves)
                    damage = enemy.damage()
                    station.setPendingDamage(damage)
                    combatResult=CombatResult::ENEMYWINS
                
                
                else
                    
                    station.move()
                    combatResult=CombatResult::STATIONESCAPES
                
                end
            
            else
                
                aLoot = enemy.loot()

                #Cuando la estación espacial vence al enemigo, al recibir el botín hay que añadir el código necesario
                #para tratar la conversión a estación eficiente o a ciudad espacial si ese botín así lo indica. En este
                #código se utilizará los dos nuevos métodos explicados anteriormente.
            
                transformation = station.setLoot(aLoot)
                if(transformation==Transformation::GETEFFICIENT)
                    combatResult = CombatResult::STATIONWINSANDCONVERTS
                    makeStationEfficient()
                elsif (transformation==Transformation::SPACECITY)
                    combatResult = CombatResult::STATIONWINSANDCONVERTS
                    createSpaceCity()
                else
                    combatResult=CombatResult::STATIONWINS
                end
                    
            end
            
            @gameState.next(@turns, @spaceStations.size())
            
            return combatResult
        end


        #Si la aplicación se encuentra en un estado en donde el combatir está
        #permitido, se realiza un combate entre la estación espacial que tiene el turno 
        #y el enemigo actual. Se devuelve el resultado del combate.
    
        def combat()

            output=CombatResult::NOCOMBAT
            state =@gameState.state()
        
            
            if ((state==GameState::BEFORECOMBAT) || (state==GameState::INIT))
                
                output= combatGo(@currentStation,@currentEnemy)
            end
            
        
            return output
        end
    
        #delegan en el método homónimo de la estación espacial que tenga
        #el turno siempre que se cumpla que el estado del juego sea INIT o 
        #AFTERCOMBAT. En caso contrario no tienen ningún efecto.
        def discardHangar
            
            if(state==GameState::INIT || state==GameState::AFTERCOMBAT)
                
                @currentStation.discardHangar
                
            end
            
        end


        #Se intenta descartar el potenciador de escudo con índice i de la
        #colección de potenciadores de escudo en uso. Además de perder el potenciador 
        #de escudo, se debe actualizar el daño pendiente (pendingDamage) si es que se 
        #tiene alguno.
    
        def discardShieldBooster(i)
            
            if(state==GameState::INIT || state==GameState::AFTERCOMBAT)
                
                @currentStation.discardShieldBooster(i)
                
            end
            
        end

    
        def discardShieldBoosterInHangar(i)
            
            if(state==GameState::INIT || state==GameState::AFTERCOMBAT)
                
                @currentStation.discardShieldBoosterInHangar(i)
                
            end
            
        end


        #Se intenta descartar el arma con índice i de la colección de armas en uso.
        #Además de perder el arma, se debe actualizar el daño pendiente (pendingDamage) 
        #si es que se tiene alguno.

        def discardWeapon(i)
            
            if(state==GameState::INIT || state==GameState::AFTERCOMBAT)
                
                @currentStation.discardWeapon(i)
                
            end
            
        end
        
        def discardWeaponInHangar(i)
            
            if(state==GameState::INIT || state==GameState::AFTERCOMBAT)
                
                @currentStation.discardWeaponInHangar(i)
                
            end
            
        end
        

        def mountShieldBooster(i)
            
            if(state==GameState::INIT || state==GameState::AFTERCOMBAT)
                
                @currentStation.mountShieldBooster(i)
                
            end
            
        end
        
        def mountWeapon(i)
            
            if(state==GameState::INIT || state==GameState::AFTERCOMBAT)
                
                @currentStation.mountWeapon(i)
                
            end
            
        end
        
    
        #Consultor
        def state
             @gameState.state()
        end

        #Capa que permite conectar el modelo con la interfaz de 
        #usuario manteniendo cierto nivel de aislamiento
        def getUIversion()
            return (GameUniverseToUI.new(@currentStation, @currentEnemy))
        end  
    
        #Devuelve true si la estación espacial que tiene el turno ha llegado al
        #número de medallas necesarias para ganar.
        def haveAWinner()
            
            output=false
            if(@currentStation.nMedals()>=@@WIN)
                output=true
            end
            
            return output
            
        end

        #Este método inicia una partida. Recibe una colección con los
        #nombres de los jugadores. Para cada jugador, se crea una estación espacial 
        #y se equipa con suministros, hangares, armas y potenciadores de escudos tomados 
        #de los mazos de cartas correspondientes. Se sortea qué jugador comienza la partida, 
        #se establece el primer enemigo y comienza el primer turno.
    
        def init(names)

            if(@gameState.state() == GameState::CANNOTPLAY)
            
                dealer = CardDealer.instance()
                #En el diagrama viene con i=1 pero creo que es asi
                for i in 0..(names.size()-1) do
                    
                    sup = dealer.nextSuppliesPackage()
                    station = SpaceStation.new(names.at(i),sup)
                    
                    #Añadimos la estación que hemos creado a spaceStations
                    @spaceStations.push(station)
                    
                    nh = @dice.initWithNHangars()
                    nw = @dice.initWithNWeapons()
                    ns = @dice.initWithNShields()
                    
                    #Creamos Loot
                    lo = Loot.new(0, nw, ns, nh, 0)
                    
                    #Lo añadimos a la estación
                    station.setLoot(lo)
                    
                end
                @currentStationIndex=@dice.whoStarts(names.size())
                @currentStation = @spaceStations[@currentStationIndex]
                @currentEnemy = dealer.nextEnemy()
                
                @gameState.next(@turns, @spaceStations.size())
      
            end


        end
    

        #Se comprueba que el jugador actual no tiene ningún daño pendiente de cumplir,
        #en cuyo caso se realiza un cambio de turno al siguiente jugador con un nuevo
        #enemigo con quien combatir, devolviendo true. Se devuelve false en otro caso.
    
        def nextTurn()
            
            output = false
        
            if(@gameState.state() == GameState::AFTERCOMBAT)
                
                if(@currentStation.validState())
                    
                    @currentStationIndex = (@currentStationIndex + 1) % @spaceStations.size()
                    @turns+=1
                    
                    @currentStation = @spaceStations.at(@currentStationIndex)
                    @currentStation.cleanUpMountedItems()
                    
                    dealer = CardDealer.instance()
                    
                    @currentEnemy = dealer.nextEnemy()
                    
                    @gameState.next(@turns, @spaceStations.size())
                    
                    output = true
                end
                    
            end
            
            return (output)

        end

        ############################################
        #Practica4
        ############################################

        def createSpaceCity
            if(!@haveSpaceCity)
            
                @currentStation = SpaceCity.new(@currentStation, @spaceStations)
                @spaceStations[@currentStationIndex] =@currentStation
                
                @haveSpaceCity = true
                
            end
        end

        def makeStationEfficient()
            @currentStation = PowerEfficientSpaceStation.new(@currentStation)
            if(@dice.extraEfficiency())
                @currentStation = BetaPowerEfficientSpaceStation.new(@currentStation)
            end
            @spaceStations[@currentStationIndex] =@currentStation
        end


        #############################################


        def to_s()
        
            spaceStations_string= "spaceStations: "
            for i in 0..(@spaceStations.size-1) do
                spaceStations_string += @spaceStations.at(i).to_s() + "\n"
            end
            
            output="\nGameUniverse attributes:"
            output+= "\ncurrentStationIndex=" +@currentStationIndex.to_s
            output+= "\nturns=" +@turns.to_s
            output+= "\ngameState=" +@gameState.to_s
            output+= "\ncurrentEnemy=" +@currentEnemy.to_s
            output+= "\ncurrentStation" +@currentStation.to_s + "\n"
            output+= spaceStations_string + "\n"

            return output

        end

        
    end


end