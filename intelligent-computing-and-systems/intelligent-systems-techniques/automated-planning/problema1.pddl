; Practica 3 - TSI
; Ejercicio 1: Viajar para desplazarse y trabajar para extraer recursos
;
; Autor: Jaime Corzo Galdó

(define (problem tierra-media-problema1) ; Cabecera del problema y nombre
  (:domain tierra-media-ej1) ; Nombre del dominio asociado al problema

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
    ; lo que no está escrito es falso
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

    ; Camino especial Rivendell-Moria. Según el enunciado, este camino se destruye
    ; al ser transitado en cualquiera de los dos sentidos, por lo que se marca como
    ; destruible en ambas direcciones.
    (caminoDestruible Rivendell Moria)
    (caminoDestruible Moria Rivendell)
  )

  ; Objetivo del ejercicio: generar recursos de tipo Mithril y Alimento. El
  ; predicado generando se logra exclusivamente mediante ExtraerRecurso, que a su
  ; vez deja constancia del personaje asignado con trabajando
  (:goal ; Objetivo a alcanzar
    (and
      (generando Mithril)
      (generando Alimento)
    )
  )
)