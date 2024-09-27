#encoding: UTF-8
require_relative 'LootToUI'

module Deepspace

    class Loot
        #getEfficient: indica si se producirá una conversión a una estación eficiente
        #spaceCity: indica si se produce conversión en ciudad espacial
        #Por defecto ambos a false
        def initialize (nSupplies, nWeapons, nShields, nHangars, nMedals, getEfficient=false,spaceCity=false)

            @nSupplies=nSupplies
            @nWeapons=nWeapons
            @nShields=nShields
            @nHangars=nHangars
            @nMedals=nMedals
            @efficient=getEfficient
            @spaceCity=spaceCity
        end

        attr_reader :nSupplies, :nWeapons, :nShields, :nHangars, :nMedals,
                    :efficient, :spaceCity 


        #Capa que permite conectar el modelo con la interfaz de 
        #usuario manteniendo cierto nivel de aislamiento
        def getUIversion()
            return (LootToUI.new(self))
        end  
        

        #Metodo publico, devuelve una representación en forma
        # de cadena de caracteres de una instancia
        def to_s()
            getUIversion().to_s()   
        end
    end

end