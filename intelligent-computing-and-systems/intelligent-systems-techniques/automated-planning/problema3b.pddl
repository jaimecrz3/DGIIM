; Practica 3 - TSI
; Ejercicio 3: se amplía la Comunidad y observamos sus efectos
;
; Autor: Jaime Corzo Galdó

(define (problem tierra-media-problema3b) ; Cabecera del problema y nombre
  (:domain tierra-media-ej3b) ; Nombre del dominio asociado al problema

  (:objects ; Nombre y tipos de los objetos del problema
    ; Personajes. Enano, Hobbit y Mago son constantes de TipoPersonaje, por lo
    ; que todos los individuos se declaran como Personaje
    Hobbit1 Hobbit2 Hobbit3 Hobbit4
    Mago1 Mago2
    Elfo1
    Enano1 Enano2 - Personaje

    Hobbiton Bree Rivendell HighPass Mirkwood Erebor
    Moria Lothlorien Tharbad Fangorn Isengard HelmsDeep
    Edoras AmonHen MinasTirith DolAmroth Tolfolas
    MinasMorgul DeadMarshes Orodruin - Localizacion
  )

  (:init ; Estado inicial
    ; Posicion inicial indicada para el ejercicio 3
    (en Hobbit1 Hobbiton)
    (en Hobbit2 Hobbiton)
    (en Hobbit3 Hobbiton)
    (en Hobbit4 Bree)
    (en Mago1 Rivendell)
    (en Mago2 Isengard)
    (en Elfo1 Lothlorien)
    (en Enano1 Moria)
    (en Enano2 Fangorn)

    ; Tipos de los personajes
    (personajeEs Hobbit1 Hobbit)
    (personajeEs Hobbit2 Hobbit)
    (personajeEs Hobbit3 Hobbit)
    (personajeEs Hobbit4 Hobbit)
    (personajeEs Mago1 Mago)
    (personajeEs Mago2 Mago)
    (personajeEs Elfo1 Elfo)
    (personajeEs Enano1 Enano)
    (personajeEs Enano2 Enano)

    ; En el ejercicio 3 todos los personajes estan disponibles inicialmente
    (disponible Hobbit1)
    (disponible Hobbit2)
    (disponible Hobbit3)
    (disponible Hobbit4)
    (disponible Mago1)
    (disponible Mago2)
    (disponible Elfo1)
    (disponible Enano1)
    (disponible Enano2)

    (esAnillo Anillo)
    (esChaleco ChalecoMithril)
    (esEspada Espada)

    (objetoEn Anillo Rivendell)
    (objetoEn Espada Lothlorien)
    (lugarDestruccion Orodruin)

    ; Capacidades de extraccion
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
    ; de los dos sentidos.
    (caminoDestruible Rivendell Moria)
    (caminoDestruible Moria Rivendell)
  )

  (:goal ; Objetivo: que el Anillo Unico haya sido destruido
    (anilloDestruido)
  )
)
