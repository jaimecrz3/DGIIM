; Practica 3 - TSI
; Ejercicio 1: Viajar para desplazarse y trabajar para extraer recursos
;
; Autor: Jaime Corzo Galdó

(define (domain tierra-media-ej1) ; Cabecera que indica el nombre del dominio
  ; Requisitos y capacidades que debe cumplir el planificador para resolver
  ; problemas con el dominio
  ; strips: logica de que las acciones solo pueden cambiar el mundo añadiendo cosas nuevas y borrando cosas que ya existían
  ; typing: para usar el guión '-' para asignar tipos a los objetos
  ; existential-preconditions: para usar el comando exists dentro de las condiciones de las acciones
  ; conditional-effects: para meter el bloque when para destrur el camino de Moria cuando se transita
  (:requirements :strips :typing :existential-preconditions :conditional-effects)

  ; TipoPersonaje se introduce para poder representar Enano y Hobbit como
  ; constantes, no como tipos PDDL. Esto evita el error tipico de declarar
  ; Enano1 - Enano cuando Enano no es un tipo, sino una constante
  (:types
    Personaje
    Localizacion
    Recurso
    TipoPersonaje
  )

  ; Constantes globales del dominio.
  ; Enano y Hobbit representan tipos de personaje a nivel del dominio.
  ; Mineral, Mithril, Madera, Especia y Alimento representan tipos de recurso.
  (:constants
    Enano Hobbit - TipoPersonaje
    Mineral Mithril Madera Especia Alimento - Recurso
  )

  (:predicates ; Predicados de la ontología
    ; (en ?p ?l): el personaje ?p esta fisicamente en la localizacion ?l
    (en ?p - Personaje ?l - Localizacion)

    ; (recursoEn ?r ?l): en la localizacion ?l existe un nodo del recurso ?r.
    ; Se usa el mismo predicado para cualquier tipo de recurso, porque Mineral,
    ; Mithril, Madera, Especia y Alimento son constantes de tipo Recurso
    (recursoEn ?r - Recurso ?l - Localizacion)

    ; (camino ?origen ?destino): existe un camino dirigido entre dos ubicaciones,
    ; en el problema se declaran ambos sentidos para representar caminos no dirigidos
    (camino ?origen ?destino - Localizacion)

    ; (caminoDestruible ?origen ?destino): indica que el camino entre dos
    ; localizaciones se destruye tras ser usado. En esta practica se utiliza
    ; para el camino Rivendell-Moria, en ambos sentidos.
    (caminoDestruible ?origen ?destino - Localizacion)

    ; (trabajando ?p ?l ?r): el personaje ?p ha sido asignado a extraer ?r en ?l.
    ; Este predicado codifica explicitamente que un personaje esta trabajando
    (trabajando ?p - Personaje ?l - Localizacion ?r - Recurso)

    ; (disponible ?p): el personaje ?p no esta trabajando y puede viajar o empezar una nueva extraccion
    (disponible ?p - Personaje)

    ; (personajeEs ?p ?t): vincula cada personaje concreto con su tipo Enano/Hobbit, lo que permite que sean constantes
    (personajeEs ?p - Personaje ?t - TipoPersonaje)

    ; (puedeExtraerTipo ?t ?r): expresa que los personajes de tipo ?t pueden
    ; extraer recursos de tipo ?r
    (puedeExtraerTipo ?t - TipoPersonaje ?r - Recurso)

    ; Predicado auxiliar de objetivo: indica que ya hay algun personaje trabajando
    ; en la generacion de un recurso de tipo ?r, de esta manera el objetivo del problema se simplifica
    (generando ?r - Recurso)
  )

  ; Accion Viajar
  ; Parametros obligatorios: Personaje, Localizacion origen, Localizacion destino
  ; Mueve a un personaje disponible entre dos localizaciones conectadas.
  ; Si el camino usado es destruible, se eliminan ambos sentidos del camino para
  ; impedir que vuelva a utilizarse posteriormente.
  (:action Viajar
    :parameters (?p - Personaje ?origen ?destino - Localizacion)
    :precondition ; El personaje debe de estar disponible, debe estar en el origen y debe existir una conexión al destino
      (and
        (disponible ?p)
        (en ?p ?origen)
        (camino ?origen ?destino)
      )
    :effect ; El personaje deja de estar en el origen y pasa a estar en el destino
      (and
        (not (en ?p ?origen))
        (en ?p ?destino)

        ; Si se cruza el camino Rivendell-Moria, queda destruido en ambos sentidos
        (when (caminoDestruible ?origen ?destino)
          (and
            (not (camino ?origen ?destino))
            (not (camino ?destino ?origen))
          )
        )
      )
  )

  ; Accion ExtraerRecurso
  ; Parametros obligatorios: Personaje, Localizacion del recurso, Tipo de recurso.
  ; Asigna un personaje disponible a un nodo de recurso compatible con su tipo.
  ; Una vez trabaja, deja de estar disponible. Ademas se marca el recurso como
  ; generandose
  (:action ExtraerRecurso
    :parameters (?p - Personaje ?l - Localizacion ?r - Recurso)
    :precondition
    ; El personaje debe de estar en la localización del recurso , estar disponible y el recurso debe existir en ese nodo,
    ; además, se comprueba si este personaje puede extraer ese recurso específico
      (and
        (disponible ?p)
        (en ?p ?l)
        (recursoEn ?r ?l)
        (exists (?t - TipoPersonaje)
          (and
            (personajeEs ?p ?t)
            (puedeExtraerTipo ?t ?r)
          )
        )
      )
    :effect ; El personaje se queda trabajando, el recurso pasa al estado de generando y el personaje pierde su disponibilidad
      (and
        (trabajando ?p ?l ?r)
        (generando ?r)
        (not (disponible ?p))
      )
  )
)