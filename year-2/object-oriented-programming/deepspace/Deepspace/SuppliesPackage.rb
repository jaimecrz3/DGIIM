#encoding: UTF-8

module Deepspace
    class SuppliesPackage

        def initialize(ammoPower,fuelUnits,shieldPower)

            @ammoPower=ammoPower
            @fuelUnits=fuelUnits
            @shieldPower=shieldPower
        end


        def ammoPower
            @ammoPower
        end

        def fuelUnits
            @fuelUnits
        end

        def shieldPower
            @shieldPower
        end


        def self.newCopy(s)
           
           return new(s.ammoPower,s.fuelUnits,s.shieldPower)
            
        end


        #Metodo publico, devuelve una representación en forma
        # de cadena de caracteres de una instancia
        def to_s()
            
            output="\nSupplies Package attributes:"
            output+= "\nammoPower="   +ammoPower.to_s
            output+= "\nfuelUnits="   +fuelUnits.to_s
            output+= "\nshieldPower=" +shieldPower.to_s

            return output
            
        end

    end

end