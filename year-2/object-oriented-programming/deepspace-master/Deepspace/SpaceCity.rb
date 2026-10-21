#encoding: UTF-8

require_relative 'SpaceStation'
require_relative 'Transformation'
require_relative 'SpaceCityToUI'

module Deepspace

    #Encapsula la mejora de una Space Station a SpaceCity.
    class SpaceCity < SpaceStation

        #base: Estacion espacial principal de la SpaceCity
        #collaborators: Resto de estaciones espaciales que componen la SpaceCity.
        def initialize(base, rest)
            newCopy(base)

            @base=base
            @collaborators=Array.new(rest)
        end

        attr_reader :collaborators

        #Se suma la potencia de disparo de todas las estaciones de la Space City.
        #Override
        def fire()
        
            fire= super

            @collaborators.each do |collaborator|
                fire+=collaborator.fire()
            end
            
            return fire

        end

        #Se suma la potencia de escudo de todas las estaciones de la Space City.
        #Override
        def protection()
        
            protection= super
            
            @collaborators.each do |collaborator|
                protection+=collaborator.protection()
            end

            return protection

        end


        #Se acepta un Loot pero se anula la posibilidad de cualquier otra 
        #transformacion.
        #Override
        def setLoot(loot)
            super
            return Transformation::NOTRANSFORM
        
        end

        #Override
        def getUIversion
            return (SpaceCityToUI.new(self))
        end
    end
end