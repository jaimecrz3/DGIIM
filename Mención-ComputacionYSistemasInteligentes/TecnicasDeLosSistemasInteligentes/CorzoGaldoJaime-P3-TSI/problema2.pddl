; Practica 3 - TSI
; Ejercicio 2: formar Comunidad reducida y destruir el anillo único
;
; Autor: Jaime Corzo Galdó

(define (problem tierra-media-problema2) ; Cabecera del problema y nombre
  (:domain tierra-media-ej2) ; Nombre del dominio asociado al problema

  (:objects ; Nombre y tipos de los objetos del problema
    ; Personajes. Enano, Hobbit y Mago son constantes de TipoPersonaje, por lo
    ; que todos los individuos se declaran como Personaje.
    Hobbit1 Hobbit2 Hobbit3 Hobbit4
    Mago1 Mago2
    Enano1 Enano2 - Personaje

    ; Localizaciones del mapa de Tierra Media.
    Hobbiton Bree Rivendell HighPass Mirkwood Erebor
    Moria Lothlorien Tharbad Fangorn Isengard HelmsDeep
    Edoras AmonHen MinasTirith DolAmroth Tolfolas
    MinasMorgul DeadMarshes Orodruin - Localizacion
  )

  (:init ; Estado inicial
    ; Estado inicial de los personajes indicado en el enunciado
    (en Hobbit1 Hobbiton)
    (en Hobbit2 Hobbiton)
    (en Hobbit3 Hobbiton)
    (en Hobbit4 Bree)
    (en Mago1 Rivendell)
    (en Mago2 Isengard)
    (en Enano1 Moria)
    (en Enano2 Isengard)

    ; Tipos de los personajes
    (personajeEs Hobbit1 Hobbit)
    (personajeEs Hobbit2 Hobbit)
    (personajeEs Hobbit3 Hobbit)
    (personajeEs Hobbit4 Hobbit)
    (personajeEs Mago1 Mago)
    (personajeEs Mago2 Mago)
    (personajeEs Enano1 Enano)
    (personajeEs Enano2 Enano)

    ; Disponibilidad. Enano2 no se declara disponible porque sigue en mision
    ; diplomatica. El resto puede actuar inicialmente
    (disponible Hobbit1)
    (disponible Hobbit2)
    (disponible Hobbit3)
    (disponible Hobbit4)
    (disponible Mago1)
    (disponible Mago2)
    (disponible Enano1)

    ; Inicialmente la Comunidad no esta formada y el Chaleco aun no existe.
    ; No se declaran comunidadFormada ni chalecoMaterializado porque, por la
    ; hipotesis de mundo cerrado, se consideran falsos al inicio

    ; Clasificacion de los objetos constantes
    (esAnillo Anillo)
    (esChaleco ChalecoMithril)
    (esEspada Espada)

    ; Posicion inicial de los objetos. El Chaleco no aparece aqui porque debe ser
    ; materializado por un Mago
    (objetoEn Anillo Rivendell)
    (objetoEn Espada Lothlorien)

    ; Lugar donde debe destruirse el Anillo
    (lugarDestruccion Orodruin)

    ; Capacidades de extraccion obtenidas del ejercicio 1
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

    ; Caminos del mapa. Se declaran en ambos sentidos
    (camino Hobbiton Bree)            (camino Bree Hobbiton)
    (camino Hobbiton Tharbad)         (camino Tharbad Hobbiton)
    (camino Bree Tharbad)             (camino Tharbad Bree)
    (camino Bree Rivendell)           (camino Rivendell Bree)
    (camino Rivendell HighPass)       (camino HighPass Rivendell)
    (camino Rivendell Moria)          (camino Moria Rivendell)
    (camino HighPass Mirkwood)        (camino Mirkwood HighPass)
    (camino Mirkwood Erebor)          (camino Erebor Mirkwood)
    (camino Moria Lothlorien)         (camino Lothlorien Moria)
    (camino Tharbad HelmsDeep)        (camino HelmsDeep Tharbad)
    (camino HelmsDeep Isengard)       (camino Isengard HelmsDeep)
    (camino HelmsDeep Edoras)         (camino Edoras HelmsDeep)
    (camino Isengard Fangorn)         (camino Fangorn Isengard)
    (camino Fangorn AmonHen)          (camino AmonHen Fangorn)
    (camino Lothlorien AmonHen)       (camino AmonHen Lothlorien)
    (camino AmonHen DeadMarshes)      (camino DeadMarshes AmonHen)
    (camino DeadMarshes MinasMorgul)  (camino MinasMorgul DeadMarshes)
    (camino MinasMorgul Orodruin)     (camino Orodruin MinasMorgul)
    (camino MinasTirith MinasMorgul)  (camino MinasMorgul MinasTirith)
    (camino Edoras MinasTirith)       (camino MinasTirith Edoras)
    (camino Edoras DolAmroth)         (camino DolAmroth Edoras)
    (camino DolAmroth Tolfolas)       (camino Tolfolas DolAmroth)
    (camino MinasTirith Tolfolas)     (camino Tolfolas MinasTirith)

    ; El camino Rivendell-Moria se destruye tras ser transitado en cualquiera
    ; de los dos sentidos
    (caminoDestruible Rivendell Moria)
    (caminoDestruible Moria Rivendell)
  )

  ; Objetivo del ejercicio 2: que el Anillo Unico haya sido destruido
  (:goal
    (anilloDestruido)
  )
)