; Practica 3 - TSI
; Ejercicio 4: crear Uruk-Hai en Torre de Hechiceria
;
; Autor: Jaime Corzo Galdo

(define (problem tierra-media-problema4) ; Cabecera del problema y nombre
  (:domain tierra-media-ej4) ; Nombre del dominio asociado al problema

  (:objects ; Nombre y tipos de los objetos del problema
    ; Personajes. Enano, Hobbit y Mago son constantes de TipoPersonaje, por lo
    ; que todos los individuos se declaran como Personaje.
    Hobbit1 Hobbit2 Hobbit3 Hobbit4
    Mago1 Mago2
    Elfo1
    Enano1 Enano2
    Humano1 Humano2
    Orco1 Orco2 Orco3
    Corsario1 Corsario2 - Personaje

    ; Localizaciones
    Hobbiton Bree Rivendell HighPass Mirkwood Erebor
    Moria Lothlorien Tharbad Fangorn Isengard HelmsDeep
    Edoras AmonHen MinasTirith DolAmroth Tolfolas
    MinasMorgul DeadMarshes Orodruin - Localizacion
  )

  (:init ; Estado inicial

    ; Posicion inicial de personajes
    ; Comunidad potencial
    (en Hobbit1 Hobbiton)
    (en Hobbit2 Hobbiton)
    (en Hobbit3 Hobbiton)
    (en Hobbit4 Bree)
    (en Mago1 Rivendell)
    (en Mago2 Isengard)
    (en Elfo1 Lothlorien)

    ; Enanos
    (en Enano1 Fangorn)
    (en Enano2 Erebor)

    ; Humanos
    (en Humano1 Edoras)
    (en Humano2 Bree)

    ; Orcos
    (en Orco1 Moria)
    (en Orco2 Moria)
    (en Orco3 Moria)

    ; Corsarios
    (en Corsario1 DolAmroth)
    (en Corsario2 DolAmroth)

    ; Tipos de personajes
    (personajeEs Hobbit1 Hobbit)
    (personajeEs Hobbit2 Hobbit)
    (personajeEs Hobbit3 Hobbit)
    (personajeEs Hobbit4 Hobbit)

    (personajeEs Mago1 Mago)
    (personajeEs Mago2 Mago)

    (personajeEs Elfo1 Elfo)

    (personajeEs Enano1 Enano)
    (personajeEs Enano2 Enano)

    (personajeEs Humano1 Humano)
    (personajeEs Humano2 Humano)

    (personajeEs Orco1 Orco)
    (personajeEs Orco2 Orco)
    (personajeEs Orco3 Orco)

    (personajeEs Corsario1 Corsario)
    (personajeEs Corsario2 Corsario)

    ; Disponibilidad inicial: Disponibles menos el Humano que esta en Edoras
    (disponible Hobbit1)
    (disponible Hobbit2)
    (disponible Hobbit3)
    (disponible Hobbit4)

    (disponible Mago1)
    (disponible Mago2)

    (disponible Elfo1)

    (disponible Enano1)
    (disponible Enano2)

    ; Humano1 no esta disponible
    (disponible Humano2)

    (disponible Orco1)
    (disponible Orco2)
    (disponible Orco3)

    (disponible Corsario1)
    (disponible Corsario2)

    ; Clasificacion de objetos
    (esAnillo Anillo)
    (esChaleco ChalecoMithril)
    (esEspada Espada)

    ; El Anillo y la Espada existen inicialmente. El Chaleco se materializa
    (objetoEn Anillo Rivendell)
    (objetoEn Espada Lothlorien)

    ; Lugar donde debe destruirse el Anillo
    (lugarDestruccion Orodruin)

    ; Capacidades de extraccion
    (puedeExtraerTipo Enano Madera)
    (puedeExtraerTipo Enano Mineral)
    (puedeExtraerTipo Enano Mithril)

    (puedeExtraerTipo Hobbit Alimento)

    (puedeExtraerTipo Orco Mineral)
    (puedeExtraerTipo Orco Madera)

    (puedeExtraerTipo Humano Madera)

    (puedeExtraerTipo Corsario Especia)

    ; Capacidades de construccion
    ; La Torre de Hechiceria la construye un Mago
    ; El Extractor lo construye un Humano
    (puedeConstruirTipo Mago TorreHechiceria)
    (puedeConstruirTipo Humano Extractor)

    ; Requisitos de edificios
    ; La Torre necesita Mineral y Madera
    (recursoNecesario TorreHechiceria Mineral)
    (recursoNecesario TorreHechiceria Madera)

    ; El Extractor necesita Madera
    (recursoNecesario Extractor Madera)

    ; Ambos edificios forman parte del proceso maligno de crear Uruk-Hai.
    ; Por eso sus recursos necesarios no pueden haber sido extraidos por Enanos
    (edificioMaligno TorreHechiceria)
    (edificioMaligno Extractor)

    ; El Mineral requiere que exista un Extractor en la misma localizacion
    ; antes de poder ser extraido
    (requiereExtractor Mineral)

    ; La Torre solo es valida en Isengard
    (edificioEnLocalizacionValida TorreHechiceria Isengard)

    ; El Extractor puede construirse en nodos de Mineral
    (edificioEnLocalizacionValida Extractor Moria)
    (edificioEnLocalizacionValida Extractor Erebor)

    ; Nodos de recursos del mundo
    (recursoEn Mineral Moria)
    (recursoEn Mithril Moria)
    (recursoEn Mineral Erebor)

    (recursoEn Madera Fangorn)
    (recursoEn Madera Lothlorien)
    (recursoEn Madera Mirkwood)

    (recursoEn Alimento Hobbiton)

    (recursoEn Especia Tolfolas)

    ; Caminos del mapa
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

    ; El camino Rivendell-Moria se destruye tras ser transitado en cualquier
    ; direccion.
    (caminoDestruible Rivendell Moria)
    (caminoDestruible Moria Rivendell)
  )

  (:goal ; Objetivo: Que el anillo sea destruido por la comunidad, que se cree un Uruk-Hai y que un humano esté en Bree
    (and
      (anilloDestruido)
      (urukHaiCreado)
      (en Humano2 Bree)
    )
  )
)