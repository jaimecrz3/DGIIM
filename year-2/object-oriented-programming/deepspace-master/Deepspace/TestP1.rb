#encoding: UTF-8
require_relative 'CombatResult'
require_relative 'GameCharacter'
require_relative 'WeaponType'
require_relative 'Loot'
require_relative 'SuppliesPackage'
require_relative 'ShieldBooster'
require_relative 'Weapon'
require_relative 'Dice'


module Deepspace
    class TestP1

        NPLAYERS=20
        REPS=100

        def main

            #CombatResult
            puts "\nCombatResult attributes:"

            puts CombatResult::ENEMYWINS
            puts CombatResult::NOCOMBAT
            puts CombatResult::STATIONESCAPES
            puts CombatResult::STATIONWINS


            #GameCharacter
            "\nGameCharacter attributes:"

            puts GameCharacter::ENEMYSTARSHIP
            puts GameCharacter::SPACESTATION
            

            #WeaponType
        
            puts "\nWeaponType attributes:\nPower:" 
            puts "LASER= "   + WeaponType::LASER.power.to_s
            puts "MISSILE= " + WeaponType::MISSILE.power.to_s
            puts "PLASMA= "  + WeaponType::PLASMA.power.to_s

            #Loot
            nSupplies=1
            nWeapons=2
            nShields=3
            nHangars=4
            nMedals=5

            loot =Loot.new(nSupplies,nWeapons,nShields,nHangars,nMedals)
            puts loot.to_s

            #Supplies Package
            ammoPower=1.0
            fuelUnits=2.0
            shieldPower=3.0
            pkg= SuppliesPackage.new(ammoPower,fuelUnits,shieldPower)
            puts pkg.to_s

            #Shield Booster
            shield_name = "The Shield"
            boost=1.0
            shield_uses=2
            shield = ShieldBooster.new(shield_name,boost,shield_uses)
            puts shield.to_s
            
            
            #Weapon
            weapon_name="The Weapon"
            weapon_uses=2
            weapon = Weapon.new(weapon_name,WeaponType::LASER,weapon_uses)
            puts weapon.to_s

            #Dice

            dice = Dice.new

            iwnh_counter0=0
            iwnh_counter1=0
            iwnw_counter1=0
            iwnw_counter2=0 
            iwnw_counter3=0
            iwns_counter0=0 
            iwns_counter1=0

            ws_counter = Array.new(NPLAYERS, 0)

            fs_counter0=0
            fs_counter1=0
            ssm_counter0=0
            ssm_counter1=0
            speed = 0.5

            for i in 1..REPS do

                #initWithNHangars
                nhangars = dice.initWithNHangars

                if nhangars == 0
                    iwnh_counter0 += 1
                else
                    iwnh_counter1 += 1
                
                end


                #initWithNWeapons
                nweapons = dice.initWithNWeapons

                if nweapons == 1
                    iwnw_counter1 += 1
                elsif weapon == 3
                        iwnh_counter2 += 1
                
                else
                    iwnw_counter3 += 1
                end

                #initWithNShields
                nshields = dice.initWithNShields

                if nshields == 0
                    iwns_counter0 += 1
                else
                    iwns_counter1 += 1
                
                end

                #whoStarts
                player = dice.whoStarts(NPLAYERS)

                ws_counter[player] += 1
                
                #firstShot
                shot= dice.firstShot
                if shot==GameCharacter::SPACESTATION
                     fs_counter0 += 1
                else 
                    fs_counter1 += 1
                
                end

                #spaceStationMoves
                move= dice.spaceStationMoves(speed)
                if move 
                    ssm_counter0 += 1
                else 
                    ssm_counter1 += 1

                end

            end

            puts "\nNumber of hangars:\n0: "+ iwnh_counter0.to_s + "\n1: "+ iwnh_counter1.to_s
            puts "\nNumber of weapons:\n1:" + iwnw_counter1.to_s + "\n2:" + iwnw_counter2.to_s + "\n3:" + iwnw_counter3.to_s
            puts "\nNumber of shields:\n0:" + iwns_counter0.to_s + "\n1:" + iwnw_counter1.to_s
        
            for i in 1..NPLAYERS do
                puts "\nPlayer " + i.to_s + " starts: " + ws_counter[i-1].to_s + " times."
            end
        
            puts "\nFirst Shot:\nSpace Station: "      + fs_counter0.to_s  + "\nEnemy Starship: " + fs_counter1.to_s
            puts "\n\nSpace Station Moves:\nEscapes: " + ssm_counter0.to_s + "\nHits: "           + ssm_counter1.to_s
            puts "\n\n"

        end

    end

    TestP1.new.main

end

