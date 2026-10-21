#encoding: UTF-8
require_relative 'ShieldBooster'
require_relative 'Weapon'
require_relative 'WeaponType'
require_relative 'Loot'
require_relative 'ShotResult'
require_relative 'Hangar'
require_relative 'Damage'
require_relative 'EnemyStarShip'
require_relative 'EnemyToUI'
require_relative 'GameUniverse'
require_relative 'SpaceStation'
require_relative 'SuppliesPackage'

module Deepspace
    class TestP2
        def main

            #Hangar
            maxElements=10
            hangar=Hangar.new(maxElements)

            #Shield Booster
            shield_name1 = "First Shield"
            shield_name2 = "Second Shield"
            boost=1.0
            shield_uses=2

            shield_1 = ShieldBooster.new(shield_name1,boost,shield_uses)
            shield_2 = ShieldBooster.new(shield_name2,boost,shield_uses)
            hangar.addShieldBooster(shield_1)
            hangar.addShieldBooster(shield_2)

            puts "-----------------------------------------------"
            puts "Adding two shield boosters...\n"
            puts hangar.shieldBoosters
            hangar.removeShieldBooster(0)
            puts "\nRemoving first shield booster...\n"
            puts hangar.shieldBoosters
            puts "\n"
        
            #Weapon
            weapon_name1= "First Weapon"
            weapon_name2 = "Second Weapon"
            weapon_uses=2

            weapon_1 = Weapon.new(weapon_name1,WeaponType::LASER,weapon_uses)
            weapon_2 = Weapon.new(weapon_name2,WeaponType::PLASMA,weapon_uses)
            hangar.addWeapon(weapon_1)
            hangar.addWeapon(weapon_2)

            puts "Adding two weapons...\n"
            puts hangar.weapons
            puts "\nRemoving first weapon...\n"
            hangar.removeWeapon(0)
            puts hangar.weapons
            puts "\n-----------------------------------------------"
            
            
            #Damage
            nShields=5
            weapons=Array.new()
            weapons.push(weapon_1)
            weapons.push(weapon_2)

            weapon_types=Array.new()
            weapon_types.push(WeaponType::LASER)
            weapon_types.push(WeaponType::PLASMA)
            weapon_types.push(WeaponType::MISSILE)
            weapon_types.push(WeaponType::LASER)

            damage=Damage.newSpecificWeapons(nShields,weapon_types)
            

            puts "El índice del vector\n"
            puts weapons
            puts "\nque contiene Plasma es: " + damage.arrayContainsType(weapons,WeaponType::PLASMA).to_s
            
            puts "\nEl vector "
            puts weapon_types
            puts "\nqueda así al eliminar la primera ocurrencia del tipo del arma pasada como parámetro (se elimina PLASMA):"
            
            damage.discardWeapon(weapon_2)
            
            puts weapon_types

            
            puts "\nDiscarding shield booster:"

            damage.discardShieldBooster
            puts "Damage shields left: #{damage.nShields}\n"

            if (damage.hasNoEffect)
                puts "Damage has no effect.\n"

            else
                puts "Damage has effect.\n\n"
            end

            puts "\n\nAjuste 1\n\n"

            puts "Damage sin ajustar: \n"
            puts damage.weapons.size().to_s + "\n"
            puts damage.to_s

            puts "Ajustamos damage...\n"

            shieldBoosters_= Array.new()
            shieldBoosters_.push(ShieldBooster.new(1,1,1))
            shieldBoosters_.push(ShieldBooster.new(2,2,2))
            shieldBoosters_.push(ShieldBooster.new(3,3,3))
            #shieldBoosters_.push(ShieldBooster.new(4,4,4))

            weapons_ = Array.new()
            weapons_.push(Weapon.new(1, WeaponType::LASER, 1))
            weapons_.push(Weapon.new(2, WeaponType::PLASMA, 2))
            #weapons_.push(Weapon.new(3, WeaponType::MISSILE, 3))
            #weapons_.push(Weapon.new(4, WeaponType::LASER, 4))

            damage_ajustado=damage.adjust(weapons_, shieldBoosters_)
            puts "Damage Ajustado: \n"
            puts damage_ajustado.to_s

            puts "\n\nAjuste 2\n\n"

            puts "Damage sin ajustar: \n"
            puts damage.to_s

            puts "Ajustamos damage...\n"

            shieldBoosters2= Array.new()
            shieldBoosters2.push(ShieldBooster.new(1,1,1))
            shieldBoosters2.push(ShieldBooster.new(2,2,2))
            shieldBoosters2.push(ShieldBooster.new(3,3,3))
            shieldBoosters2.push(ShieldBooster.new(4,4,4))
            shieldBoosters2.push(ShieldBooster.new(5,5,5))
            shieldBoosters2.push(ShieldBooster.new(6,6,6))

            weapons2 = Array.new()
            weapons2.push(Weapon.new(1, WeaponType::LASER, 1))
            weapons2.push(Weapon.new(2, WeaponType::MISSILE, 2))
            weapons2.push(Weapon.new(3, WeaponType::LASER, 3))
            weapons2.push(Weapon.new(4, WeaponType::PLASMA, 4))
            #weapons_.push(Weapon.new(3, WeaponType::MISSILE, 3))
            #weapons_.push(Weapon.new(4, WeaponType::LASER, 4))

            damage_ajustado2=damage.adjust(weapons2, shieldBoosters2)

            puts "Damage Ajusado: \n"
            puts damage_ajustado2.to_s


            #Enemy Starship

            es_name="Enemy Starship"
            puts damage
            es_ammoPower=2.0
            es_shieldPower=2.0

            nSupplies=1
            nWeapons=2
            nShields=3
            nHangars=4
            nMedals=5
            es_loot=Loot.new(nSupplies,nWeapons,nShields,nHangars,nMedals)

            es_damage=Damage.newCopy(damage)

            enemystarship=EnemyStarShip.new(es_name,es_ammoPower,es_shieldPower,es_loot,es_damage)

            #EnemytoUI del EnemyStarship

            enemytoui = EnemyToUI.new(enemystarship)

            shot_1=1
            shot_2=3

           
            puts "First Shot: #{shot_1}: " + "#{enemystarship.receiveShot(shot_1)}\n"
            puts "Second Shot: #{shot_2}: " + "#{enemystarship.receiveShot(shot_2)}\n"


            #GameUniverse

            puts "\n\n"
            puts "Creamos GameUniverse\n\n"

            gameuniverse = GameUniverse.new

            #gameuniverse.mountShieldBooster(ShieldBooster.new(1,1,1))
            #gameuniverse.mountWeapon(Weapon.new(1, WeaponType::LASER, 1))

            puts gameuniverse.to_s

            #SpaceStation

            puts "\n\n"
            puts "Creamos SpaceStation\n\n"

            
            suppliespackage = SuppliesPackage.new(1,2,3)
            spacestation = SpaceStation.new("SS", suppliespackage)

            puts "Asignamos fuel:\n"
            spacestation.assignFuelValue(101)
            puts "fuel: #{spacestation.fuelUnits}"
            
            puts "\nVelocidad de la estación:  #{spacestation.speed}\n"
            
            puts"\nLe pasamos un hangar y montamos armas y shieldBoosters\n"
            puts hangar.to_s
            spacestation.receiveHangar(hangar)
            spacestation.mountShieldBooster(0)
            spacestation.mountWeapon(0)
            puts spacestation.to_s

            puts "\nAñadimos armas y potenciadores de escudo al hangar:\n"
            puts spacestation.receiveShieldBooster(shield_1)
            puts spacestation.receiveWeapon(weapon_1)
            puts spacestation.to_s

            puts "\La estación tiene un estado válido: #{spacestation.validState}\n"

            puts "\nAjustamos el daño:\n"
            spacestation.mountWeapon(0)
            spacestation.setPendingDamage(damage)
            puts spacestation.to_s

            puts "\La estación tiene un estado válido: #{spacestation.validState}\n"

            puts "\nAñadimos un arma y un potenciador de escudo con cero usos\n"

            spacestation.receiveWeapon(Weapon.new("Third Weapon", WeaponType::LASER, 0))
            spacestation.mountWeapon(0)
            spacestation.receiveShieldBooster(ShieldBooster.new("Third Shield", 1.0, 0))
            spacestation.mountShieldBooster(1)
            puts spacestation.to_s

            puts "\nEliminamos las armas y los escudos que no tienen efecto: \n"
            
            spacestation.cleanUpMountedItems
            puts spacestation.to_s

        end
            
    end

    TestP2.new.main
end
