#encoding: UTF-8

require_relative 'GameCharacter'

module Deepspace
    class Dice

        def initialize()
            @NHANGARSPROB=0.25
            @NSHIELDSPROB=0.25
            @NWEAPONSPROB=0.33
            @FIRSTSHOTPROB=0.5
            @EXTRAEFFICIENCYPROB=0.8
            @generator = Random.new

        end

        def initWithNHangars
        
            prob= @generator.rand() #Genera un numero entre [0,1)
        
            if (prob < @NHANGARSPROB) 
                output=0
            else output=1;
            
            end
            
            return output
        
        end

        def initWithNWeapons
        
            prob= @generator.rand() #Genera un numero entre [0,1)
        
            if (prob < @NHANGARSPROB) 
                output=1
            elsif (prob>=@NWEAPONSPROB && prob < 2*@NWEAPONSPROB) 
                    output=2
                
            else
                output=3
            end
            
            return output
        
        end

        def initWithNShields
        
            prob= @generator.rand() #Genera un numero entre [0,1)
        
            if (prob < @NSHIELDSPROB) 
                output=0
            else output=1;
            
            end
            
            return output
        
        end

        def whoStarts(nPlayers)
        
            prob= @generator.rand(nPlayers); #Genera un numero entre [0,nPlayers) (entero)
        
        end

        def firstShot

            prob= @generator.rand() #Genera un numero entre [0,1)

            if (prob < @NSHIELDSPROB) 
                output=GameCharacter::SPACESTATION
            else 
                output=GameCharacter::ENEMYSTARSHIP

            end

            return output


        end

        def spaceStationMoves(speed)

            prob= @generator.rand() #Genera un numero entre [0,1)
        
            if (prob < speed) 
                output=true
            else 
                output=false
            
            end
            
            return output

        end

        def extraEfficiency()
            prob= @generator.rand
        
            if(prob < @EXTRAEFFICIENCYPROB)
                output=true
            else
                output=false
            end
            
            return output
        end

        #Metodo publico, devuelve una representación en forma
        # de cadena de caracteres de una instancia
        def to_s()
            
            output= "Dice attributes:\n"      
            output+="NHANGARS probability: "  + NHANGARSPROB + "\n" 
            output+= "NSHIELD probability: "  + NSHIELDSPROB + "\n" 
            output+= "NWEAPONS probability: " + NWEAPONSPROB + "\n" 
            output+= "FIRSTSHOT probability: " + FIRSTSHOTPROB + "\n" 
            output+= "Generator: "             + generator.to_s + "\n";
        
        return output
            
        end

    end

end

    
        
