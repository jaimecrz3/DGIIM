#encoding: UTF-8

require_relative 'SpaceStationToUI'
require_relative 'SuppliesPackage'
require_relative 'Hangar'
require_relative 'Damage'
require_relative 'Weapon'
require_relative 'WeaponType'
require_relative 'Loot'
require_relative 'ShieldBooster'
require_relative 'CardDealer'
require_relative 'ShotResult'
require_relative 'Transformation'

module Deepspace

    class SpaceStation

        #Maximo de unidades combustible que puede tener una estacion.
        #Es de clase
        @@MAXFUEL = 100

        #Unidades de escudo perdidas por cada unidad de potencia de disparo recicbido.
        #Es de clase
        @@SHIELDLOSSPERUNITSHOT = 0.1

        #Getters de clase
        
        def self.MAXFUEL
            @@MAXFUEL
        end

        def self.SHIELDLOSSPERUNITSHOT
            @@SHIELDLOSSPERUNITSHOT
        end


        #Constructores
        def initialize(n, supplies)

            @ammoPower=0.0
            @fuelUnits=0.0
            @name=n
            @nMedals=0
            @shieldPower=0.0
            @weapons=Array.new()
            @shieldBoosters=Array.new()
            @hangar=nil
            @pendingDamage=nil

            self.receiveSupplies(supplies)

        end

        #Contructor de copia
        #
        #
        #NO LO TENGO CLARO
        #
        #
        #
        def newCopy(station)
        
            @name = station.name
            @ammoPower = station.ammoPower
            @shieldPower = station.shieldPower
            @fuelUnits = station.fuelUnits
            
			@nMedals=station.nMedals
			@weapons=station.weapons.clone()
			@shieldBoosters=station.shieldBoosters.clone()
			
            if !station.hangar.nil?
                @hangar=Hangar.newCopy(station.hangar)
            end
            
            if !station.pendingDamage.nil?
                @pendingDamage=station.pendingDamage.copy()
            end
            
        end

        #Realiza un disparo y se devuelve la energía o potencia del mismo. Para ello 
        #se multiplica la potencia de disparo por los factores potenciadores 
        #proporcionados por todas las armas
        def fire
            factor=1.0

            @weapons.each do |weapon|
                factor *= weapon.useIt
            end
    
            return @ammoPower*factor
        end
            
        #Getters
        def ammoPower
            @ammoPower
        end

            
        def fuelUnits
            @fuelUnits
        end
            

        def hangar 
            @hangar
        end


        def name
            @name
        end
            

        def nMedals
            @nMedals
        end

            
        def pendingDamage
            @pendingDamage
        end

            
        def shieldBoosters 
            @shieldBoosters
        end   

        def shieldPower
            @shieldPower
        end

        def weapons
            @weapons
        end
            
            
            
        #Devuelve la velocidad de la estacion, que se calcula como la fracción entre 
        #las unidades de combustible de las que dispone en la actualidad la estación 
        #espacial respecto al máximo unidades de combustible que es posible almacenar.
        
            
        def speed
            return (@fuelUnits/SpaceStation.MAXFUEL)
        end
            
            
        #Interfaz

        def getUIversion
            return (SpaceStationToUI.new(self))
        end



        #La potencia de disparo, la del escudo y las unidades de
        #combustible se incrementan con el contenido del paquete de suministro.
        
        def receiveSupplies(s)
            
            @ammoPower += s.ammoPower
            @shieldPower += s.shieldPower
            self.assignFuelValue(@fuelUnits + s.fuelUnits)
        end

            
            
        #Fija la cantidad de combustible al valor pasado como parámetro sin
        #que nunca se exceda del límite.

        def assignFuelValue(f)
            
            if(f>SpaceStation.MAXFUEL) 
                f=SpaceStation.MAXFUEL
            end

            @fuelUnits=f
        end

        
        #Si pendingDamage no tiene efecto lo fija a null.

        def cleanPendingDamage
            if(@pendingDamage.hasNoEffect) 
                @pendingDamage=nil
            end
        end
        
        
        #Eliminar todas las armas y los potenciadores de escudo montados a las que no 
        #les queden usos.

        def cleanUpMountedItems
            
            @weapons = @weapons.select {|weapons| weapons.uses>0}
            @shieldBoosters = @shieldBoosters.select {|sb| sb.uses>0}

           
        end
        

        #Fija hangar a null para indicar que no se dispone de él.
        def discardHangar
            @hangar=nil
        end
        
        #Se intenta descartar el potenciador de escudo con índice i de la
        #colección de potenciadores de escudo en uso. Además de perder el potenciador de escudo, se debe
        #actualizar el daño pendiente (pendingDamage) si es que se tiene alguno.
        def discardShieldBooster(i) 
            
            if(i >= 0 && i < @shieldBoosters.size())
            
                @shieldBoosters.delete_at(i);
                
                    if(@pendingDamage != nil)
                        
                        @pendingDamage.discardShieldBooster();
                        
                        self.cleanPendingDamage();
                        
                    end
                end
        
        end
        
        
        #Si se dispone de hangar, se solicita al mismo descartar el potenciador de 
        #escudo con índice i.

        def discardShieldBoosterInHangar(i)
            if (@hangar!=nil) 
                @hangar.removeShieldBooster(i)
            end
        end


        
        #Se intenta descartar el arma con índice i de la colección de armas en uso.
        #Además de perder el arma, se debe actualizar el daño pendiente (pendingDamage) 
        #si es que se tiene alguno.
        
        def discardWeapon(i) 
        
            if(i >= 0 && i < @weapons.size())
            
                w= Weapon.newCopy(@weapons.delete_at(i))
            
                if(@pendingDamage != nil)
                    
                    @pendingDamage.discardWeapon(w)
                    
                    self.cleanPendingDamage()
                    
                end
            end
            
        
        
        end
        

        #Si se dispone de hangar, se solicita al mismo descartar el arma con índice i.
        
        def discardWeaponInHangar(i)
            if (@hangar!=nil) 
                @hangar.removeWeapon(i)
            end
            
        end
        


        #Setters        
        
        
        #Se intenta montar el potenciador de escudo con el índice i dentro del hangar. 
        #Si se dispone de hangar, se le indica que elimine el arma de esa posición y 
        #si esta operación tiene éxito (el hangar proporciona el potenciador), se 
        #añade el potenciador a la colección de armas en uso.
        
        def mountShieldBooster(i)
            
            if (@hangar!=nil)
                
                shieldbooster=@hangar.removeShieldBooster(i)
                if (shieldbooster!=nil) 
                    @shieldBoosters.push(shieldbooster)
                end
            end
        end
        
        
        
        #Se intenta montar el arma con el índice i dentro del hangar. Si se dispone
        #de hangar, se le indica que elimine el arma de esa posición y si esta 
        #operación tiene éxito (el hangar proporciona el arma), se añade el arma a la 
        #colección de armas en uso.
        
        def mountWeapon(i)
        
            if (@hangar!=nil)
                
                weapon=@hangar.removeWeapon(i)
                if (weapon!=nil) 
                    @weapons.push(weapon)
                end
            end
        end
        
        
        
        #Decremento de unidades de combustible disponibles a causa de un desplazamiento. 
        #Al número de las unidades almacenadas se les resta una fracción de las 
        #mismas que es igual a la velocidad de la estación. Las unidades de combustible 
        #no pueden ser inferiores a 0.
        
        def move()
            
            @fuelUnits-= speed
            
            if(@fuelUnits<0)
                @fuelUnits=0
            end
        end
        
        #Se usa el escudo de protección y se devuelve la energía del mismo. Para ello
        #se multiplica la potencia del escudo por los factores potenciadores 
        #proporcionados por todos los potenciadores de escudos de los que se dispone.
        def protection
            factor=1.0

            @shieldBoosters.each do |sb|
                factor *= sb.useIt
            end
    
            return @shieldPower*factor
        end
        
        
        #Si no se dispone de hangar, el parámetro pasa a ser el hangar de la
        #estación espacial. Si ya se dispone de hangar esta operación no tiene efecto.
        
        
        def receiveHangar(h)
            if(@hangar==nil)
                @hangar=h
            end
        end
        
        
        #Si se dispone de hangar, devuelve el resultado de intentar
        #añadir el potenciador de escudo al mismo. En caso contrario false.
        
        def receiveShieldBooster(s)
        
            received=false
            
            if (@hangar!=nil) 
                received=@hangar.addShieldBooster(s)
            end
            
            return received
        end
        
        #Realiza las operaciones relacionadas con la recepción del impacto de un 
        #disparo enemigo. Ello implica decrementar la potencia del escudo en función 
        #de la energía del disparo recibido como parámetro y devolver el resultado de
        #si se ha resistido el disparo o no.
        def receiveShot(shot) 
            output= ShotResult::DONOTRESIST
    
            if (protection()>=shot)
            
                @shieldPower-=@@SHIELDLOSSPERUNITSHOT*shot
                if (@shieldPower < 0) 
                    @shieldPower=0.0
                end
                
                output = ShotResult::RESIST
        
            else @shieldPower=0.0
            end
            
            return output
        end
        
    
        #Si se dispone de hangar, devuelve el resultado de intentar
        #añadir el arma al mismo. En caso contrario false.
        
        def receiveWeapon(w)
            
            received=false
            
            if (@hangar!=nil) 
                received=@hangar.addWeapon(w)
            end
            
            return received
        end


        #Recepción de un botín. Por cada elemento que indique el botín (pasado
        #como parámetro) se le pide a CardDealer un elemento de ese tipo y se intenta almacenar con el
        #método receive*() correspondiente. Para las medallas, simplemente se incrementa su número según
        #lo que indique el botín.
        def setLoot(loot) 
            dealer = CardDealer.instance
        
            if(loot.nHangars()>0)
                
                hangar_ = dealer.nextHangar()
                self.receiveHangar(hangar_)
                
            end

            for i in 0..(loot.nSupplies()-1) do
            
            sup = dealer.nextSuppliesPackage()
            self.receiveSupplies(sup);
            end
            
        
            for i in 0..(loot.nWeapons()-1) do
                
                weap = dealer.nextWeapon()
                self.receiveWeapon(weap)
                
            end
            
            for i in 0..(loot.nShields()-1) do
                
                sh = dealer.nextShieldBooster()
                self.receiveShieldBooster(sh)
                
            end
            
            @nMedals += loot.nMedals()

            transformation=Transformation::NOTRANSFORM
            if(loot.efficient()) 
                transformation=Transformation::GETEFFICIENT
            elsif(loot.spaceCity()) 
                transformation=Transformation::SPACECITY
            end
        
            return transformation
    
        end
        
        
        #Se calcula el parámetro ajustado (adjust) a la lista de armas y
        #potenciadores de escudo de la estación y se almacena el resultado en el 
        #atributo correspondiente.
        
        def setPendingDamage(d)
            if (!d.nil?) 
                @pendingDamage=d.adjust(@weapons, @shieldBoosters)
            end
        end
        
        
        
        #Devuelve true si la estación espacial está en un estado válido. Eso implica 
        #que o bien no se tiene ningún daño pendiente o que este no tiene efecto.
        
        def validState
            return (@pendingDamage==nil || @pendingDamage.hasNoEffect)
        end
        
        def to_s()
    
            weapons_string= "Weapons: "
            for i in 0..(@weapons.size-1) do
                weapons_string += @weapons.at(i).to_s() + " "
            end


            shieldBoosters_string= "ShieldBoosters: "
            for i in 0..(@shieldBoosters.size-1) do
                shieldBoosters_string += @shieldBoosters.at(i).to_s() + "\n"
            end
            
            output="\nSpaceStation attributes:"
            output+= "\nammoPower=" +@ammoPower.to_s
            output+= "\nfuelUnits=" +@fuelUnits.to_s
            output+= "\nname=" +@name.to_s
            output+= "\nnMedals=" +@nMedals.to_s
            output+= "\nshieldPower" +@shieldPower.to_s + "\n"
            output+= weapons_string + "\n"
            output+= shieldBoosters_string + "\n"
            output+= hangar.to_s + pendingDamage.to_s + "\n"

            return output

        end
        
    end
end

=begin
space_station = Deepspace::SpaceStation.new("estacion", Deepspace::SuppliesPackage.new(1.0, 1.0, 1.0))

hangar = Deepspace::Hangar.new(10)

    
shield_name1 = "First Shield"
shield_name2 = "Second Shield"
boost=1.0
shield_uses=2

shield_1 = Deepspace::ShieldBooster.new(shield_name1,boost,shield_uses)
shield_2 = Deepspace::ShieldBooster.new(shield_name2,boost,shield_uses)
hangar.addShieldBooster(shield_1)
hangar.addShieldBooster(shield_2)

weapon_name1= "First Weapon"
weapon_name2 = "Second Weapon"
weapon_uses=2

weapon_1 = Deepspace::Weapon.new(weapon_name1,Deepspace::WeaponType::LASER,weapon_uses)
weapon_2 = Deepspace::Weapon.new(weapon_name2,Deepspace::WeaponType::PLASMA,weapon_uses)
hangar.addWeapon(weapon_1)
hangar.addWeapon(weapon_2)

space_station.receiveHangar(hangar)

puts space_station.getUIversion
puts "\n-----------------------------------------------------\n"

space_station_copy = Deepspace::SpaceStation.new("estacion_copia", Deepspace::SuppliesPackage.new(2.0, 2.0, 2.0))
space_station_copy.newCopy(space_station)

puts space_station_copy.getUIversion

=end



