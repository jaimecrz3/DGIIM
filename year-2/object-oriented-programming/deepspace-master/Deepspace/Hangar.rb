#encoding: UTF-8

require_relative 'HangarToUI'

module Deepspace
    class Hangar 

        #Constructores
        def initialize (maxElements)
            @maxElements=maxElements 
            @weapons=Array.new()
            @shieldBoosters=Array.new()
        end

        # def self.newHangar(maxElements)
        #    new(maxElements,Array.new,Array.new)
        #end


        #Getters
        def maxElements
            @maxElements
        end

        def shieldBoosters
            @shieldBoosters
        end

        def weapons
            @weapons
        end

        #Constructor de copia
        #Tiene que devolver el objeto pq el constructor de copia en Ruby se hace llamando a un metodo de clase, luego haces 
        #objeto = Hangar.new_copy(s) que realmente es una asignación
        def self.newCopy(s)
            copy = new(s.maxElements)

            s.weapons.each do |weapon|
                copy.addWeapon(weapon)
            end

            s.shieldBoosters.each do |shieldBoosters|
                copy.addShieldBooster(shieldBoosters)
            end

            return copy

        end
 

        #Capa que permite conectar el modelo con la interfaz de 
        #usuario manteniendo cierto nivel de aislamiento
        def getUIversion()
            return (HangarToUI.new(self))
        end  



        #Comprueba si hay espacio disponible en el hangar
        def spaceAvailable
            available=false
            if ((@weapons.size + @shieldBoosters.size) < @maxElements) 
                available=true
            end
        
            return available
        end

        private :spaceAvailable  #Para hacerlo privado

        #Añade un arma si hay espacio disponible
        def addWeapon(w)
            available=false
            if (spaceAvailable)
                @weapons.push(w)
                available=true
            end
        
            return available
        end

        #Añade un escudo si hay espacio disponible
        def addShieldBooster(s) 
            available=false
            if (spaceAvailable)
                @shieldBoosters.push(s)
                available=true
            end
        
            return available
        end

        
 
        #Elimina el escudo de la pos 's' y lo devuelve
        #Si no esta devuelve nill
        def removeShieldBooster(s) 
            shield=nil
        
            if (0<=s && s < @shieldBoosters.size)
                shield=@shieldBoosters.at(s)    #Acceder al elemento w
                @shieldBoosters.delete_at(s)    #Borrar el elemento w
            end
        
            return shield
        end

        #Elimina el arma de la pos 'w' y lo devuelve
        #Si no esta devuelve nill
        def removeWeapon(w)
            weapon=nil
        
            if (0<=w && w < @weapons.size)
                weapon=@weapons.at(w)    #Acceder al elemento w
                @weapons.delete_at(w)    #Borrar el elemento w
            end
        
            return weapon
        end

        def to_s()
        
            weapons_string= "Weapons: "
            for i in 0..(@weapons.size-1) do
                weapons_string += @weapons.at(i).to_s() + "\n"
            end

            shieldBoosters_string= "shieldBoosters: "
            for i in 0..(@shieldBoosters.size-1) do
                shieldBoosters_string += @shieldBoosters.at(i).to_s() + "\n"
            end
            
            output="\nHangar attributes:"
            output+= "\nmaxElements=" +@maxElements.to_s + "\n"
            output+= weapons_string + "\n"
            output+= shieldBoosters_string + "\n"

            return output

        end

    end

end

