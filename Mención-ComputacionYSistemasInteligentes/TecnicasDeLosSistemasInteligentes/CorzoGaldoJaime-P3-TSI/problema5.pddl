; Practica 3 - TSI
; Ejercicio 5: acciones con costes variables
;
; Autor: Jaime Corzo Galdo

(define (problem tierra-media-problema5) ; Cabecera del problema y nombre
  (:domain tierra-media-ej5) ; Nombre del dominio asociado al problema

  (:objects ; Nombre y tipos de los objetos del problema
    ; Indicamos los personajes del ejercicio 1
    Enano1 Enano2 Hobbit1 - Personaje

    ; Indicamos las localizaciones del mapa de Tierra Media
    Hobbiton Bree Rivendell HighPass Mirkwood Erebor
    Moria Lothlorien Tharbad Fangorn Isengard HelmsDeep
    Edoras AmonHen MinasTirith DolAmroth Tolfolas
    MinasMorgul DeadMarshes Orodruin - Localizacion
  )

  (:init ; Estado inicial

    ; Inicializacion de la funcion de coste a cero, aqui se va a ir almacenando
    ; el contador del coste que ira aumentando en cada paso
    (= (total-cost) 0)

    ; Situacion inicial de los personajes.
    (en Enano1 Tharbad)
    (en Hobbit1 Lothlorien)
    (en Enano2 Isengard)

    ; Tipos de los personajes.
    (personajeEs Enano1 Enano)
    (personajeEs Enano2 Enano)
    (personajeEs Hobbit1 Hobbit)

    ; Disponibilidad: Enano2 no se declara disponible porque esta en una mision
    ; diplomatica. Por la hipotesis del mundo cerrado, eso lo hace no disponible
    (disponible Enano1)
    (disponible Hobbit1)

    ; Capacidades de extraccion por tipo de personaje segun el enunciado
    (puedeExtraerTipo Enano Madera)
    (puedeExtraerTipo Enano Mineral)
    (puedeExtraerTipo Enano Mithril)
    (puedeExtraerTipo Hobbit Alimento)

    ; Nodos de recursos del mundo
    (recursoEn Mineral Moria)
    (recursoEn Mithril Moria)
    (recursoEn Mineral Erebor)
    (recursoEn Madera Fangorn)
    (recursoEn Madera Lothlorien)
    (recursoEn Madera Mirkwood)
    (recursoEn Alimento Hobbiton)
    (recursoEn Especia Tolfolas)

    ; Caminos del mapa. Se declaran en ambos sentidos para representar conexiones
    ; bidireccionales, ya que de lo contrario, por la hipotesis del mundo cerrado
    ; lo que no está escrito es falso.
    ; Ahora en lugar de usar el predicado genérico (camino ?origen ?destino), los costes de los caminos
    ; están clasificados explícitamente (por ejemplo, cruzar las Montañas Nubladas por el High Pass cuesta 2, mientras que ir de Fangorn a Amon Hen cuesta 8)
    ;
    ; Caminos de coste 1
    (caminoCoste1 Hobbiton Bree)        (caminoCoste1 Bree Hobbiton)
    (caminoCoste1 Hobbiton Tharbad)     (caminoCoste1 Tharbad Hobbiton)

    (caminoCoste1 Bree Tharbad)         (caminoCoste1 Tharbad Bree)
    (caminoCoste1 Bree Rivendell)       (caminoCoste1 Rivendell Bree)

    (caminoCoste1 HelmsDeep Isengard)   (caminoCoste1 Isengard HelmsDeep)
    (caminoCoste1 HelmsDeep Edoras)     (caminoCoste1 Edoras HelmsDeep)

    (caminoCoste1 Isengard Fangorn)     (caminoCoste1 Fangorn Isengard)

    (caminoCoste1 DolAmroth Tolfolas)   (caminoCoste1 Tolfolas DolAmroth)

    ; Caminos de coste 2
    (caminoCoste2 Rivendell HighPass)       (caminoCoste2 HighPass Rivendell)
    (caminoCoste2 HighPass Mirkwood)        (caminoCoste2 Mirkwood HighPass)
    (caminoCoste2 Mirkwood Erebor)          (caminoCoste2 Erebor Mirkwood)

    (caminoCoste2 AmonHen DeadMarshes)      (caminoCoste2 DeadMarshes AmonHen)
    (caminoCoste2 DeadMarshes MinasMorgul)  (caminoCoste2 MinasMorgul DeadMarshes)

    (caminoCoste2 MinasTirith MinasMorgul)  (caminoCoste2 MinasMorgul MinasTirith)

    (caminoCoste2 Edoras MinasTirith)       (caminoCoste2 MinasTirith Edoras)
    (caminoCoste2 MinasTirith Tolfolas)     (caminoCoste2 Tolfolas MinasTirith)
    (caminoCoste2 Edoras DolAmroth)         (caminoCoste2 DolAmroth Edoras)

    ; Caminos de coste 3
    (caminoCoste3 Tharbad HelmsDeep)        (caminoCoste3 HelmsDeep Tharbad)
    (caminoCoste3 MinasMorgul Orodruin)     (caminoCoste3 Orodruin MinasMorgul)
    (caminoCoste3 Moria Lothlorien)         (caminoCoste3 Lothlorien Moria)

    ; Caminos de coste 5
    (caminoCoste5 Lothlorien AmonHen)       (caminoCoste5 AmonHen Lothlorien)

    ; Caminos de coste 8
    (caminoCoste8 Fangorn AmonHen)          (caminoCoste8 AmonHen Fangorn)

    ; Camino especial Rivendell-Moria. Tiene coste 3 y se destruye al cruzarse.
    (caminoCoste3Destruible Rivendell Moria)
    (caminoCoste3Destruible Moria Rivendell)
  )

  ; El objetivo es el mismo que en el Ejercicio 1: generar Mithril y Alimento.
  (:goal
    (and
      (generando Mithril)
      (generando Alimento)
    )
  )

  ; En este ejercicio ya no se minimiza simplemente el numero de acciones,
  ; sino el coste total acumulado por las acciones ejecutadas. Esto le dice a fast downward
  ; que para certificar que el plan es óptimo el valor final de total-cost tiene que ser mínimo
  (:metric minimize (total-cost))
)