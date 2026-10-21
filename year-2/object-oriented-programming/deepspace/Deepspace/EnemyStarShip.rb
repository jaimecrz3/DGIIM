#encoding: UTF-8
require_relative 'EnemyToUI'
module Deepspace;

    #Esta clase representa a una nave espacial enemiga.
    class EnemyStarShip
        
        #Constructores
        def initialize(n, a, s, l, d)
            
            #Nombre
            @name=n
            #Poder de disparo
            @ammoPower=a
            #Poder de escudo
            @shieldPower=s
            #Botín
            @loot=l
            #Daño
            @damage=d
        end


        #Getters
        
        def fire
            @ammoPower
        end
        
        def ammoPower
            @ammoPower
        end
        
        def damage
            @damage
        end
        
        def loot
            @loot
        end

        def name
            @name
        end
        
        def shieldPower
            @shieldPower
        end
        
        def protection
            @shieldPower
        end
        


        
        def self.newCopy (e)
            return new(e.name,e.ammoPower,e.shieldPower,e.loot,e.damage)
        end
        
        
        #Interfaz
        def getUIversion
            return (EnemyToUI.new(self))
        end
        
        
        #Others
        
        
        #Devuelve el resultado que se produce al recibir un disparo de una determinada 
        #potencia (pasada como parámetro). 
        
        #Si el nivel de la protección de los escudos es menor que la intensidad del 
        #disparo, la nave enemiga no resiste (DONOTRESIST).
        #En casocontrario resiste el disparo (RESIST).
    
        
        def receiveShot(shot)
            
            result=ShotResult::RESIST
            
            if (@shieldPower < shot) 
                result=ShotResult::DONOTRESIST
            end
            
            return result
        end

        def to_s()
            
            output="\nEnemyStarship attributes:"
            output+= "\nname=" +@name.to_s
            output+= "\nammoPower=" +@ammoPower.to_s
            output+= "\nLoot=" +@loot.to_s
            output+= "\nDamage=" +@damage.to_s + "\n"

            return output

        end

    end
end
