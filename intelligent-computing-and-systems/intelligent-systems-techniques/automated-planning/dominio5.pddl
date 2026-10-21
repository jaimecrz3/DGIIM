; Practica 3 - TSI
; Ejercicio 5: acciones con costes variables
;
; Autor: Jaime Corzo Galdo

(define (domain tierra-media-ej5) ; Cabecera que indica el nombre del dominio
  ; Requisitos y capacidades que debe cumplir el planificador para resolver
  ; problemas con el dominio
  ; strips: logica de que las acciones solo pueden cambiar el mundo añadiendo cosas nuevas y borrando cosas que ya existían
  ; typing: para usar el guión '-' para asignar tipos a los objetos
  ; existential-preconditions: para usar el comando exists dentro de las condiciones de las acciones
  ; action-costs: Para poder definir de manera manual el coste de cada accion
  (:requirements :strips :typing :existential-preconditions :action-costs)

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

    ; Caminos clasificados por coste. Se sustituyen los caminos genericos del Ejercicio 1 por predicados separados,
    ; ya que cada grupo de caminos incrementa total-cost con un valor distinto
    (caminoCoste1 ?origen ?destino - Localizacion)
    (caminoCoste2 ?origen ?destino - Localizacion)
    (caminoCoste3 ?origen ?destino - Localizacion)
    (caminoCoste5 ?origen ?destino - Localizacion)
    (caminoCoste8 ?origen ?destino - Localizacion)

    ; Camino de coste 3 que se destruye al cruzarse.
    ; Se usa para Rivendell-Moria, que desaparece en ambos sentidos tras ser usado.
    (caminoCoste3Destruible ?origen ?destino - Localizacion)

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

  ; Funcion numerica especial requerida por la directiva :action-costs para ir acumulando el coste total del plan
  (:functions
    (total-cost)
  )

  ; Accion Viajar
  ; Parametros: Personaje, localizacion origen, localizacion destino.
  ;
  ; Esta accion se usa para todos los desplazamientos de coste 1.
  ; Conservamos el nombre Viajar para mantener la accion principal del Ejercicio 1
  (:action Viajar
    :parameters (?p - Personaje ?origen ?destino - Localizacion)
    :precondition
      (and
        (disponible ?p)
        (en ?p ?origen)
        (caminoCoste1 ?origen ?destino)
      )
    :effect
      (and
        (not (en ?p ?origen))
        (en ?p ?destino)
        (increase (total-cost) 1)
      )
  )

  ; Variante de Viajar para caminos de coste 2.
  ; Se introduce por la limitacion de Fast Downward: no se puede incrementar
  ; total-cost con una funcion variable del estilo (coste ?origen ?destino).
  (:action ViajarCoste2
    :parameters (?p - Personaje ?origen ?destino - Localizacion)
    :precondition
      (and
        (disponible ?p)
        (en ?p ?origen)
        (caminoCoste2 ?origen ?destino)
      )
    :effect
      (and
        (not (en ?p ?origen))
        (en ?p ?destino)
        (increase (total-cost) 2)
      )
  )

  ; Variante de Viajar para caminos de coste 3
  (:action ViajarCoste3
    :parameters (?p - Personaje ?origen ?destino - Localizacion)
    :precondition
      (and
        (disponible ?p)
        (en ?p ?origen)
        (caminoCoste3 ?origen ?destino)
      )
    :effect
      (and
        (not (en ?p ?origen))
        (en ?p ?destino)
        (increase (total-cost) 3)
      )
  )

  ; Variante especial de Viajar para el camino destruible Rivendell-Moria.
  ; Tiene coste 3, pero además elimina el camino en ambos sentidos para que no
  ; pueda volver a utilizarse durante el plan.
  (:action ViajarCoste3Destruible
    :parameters (?p - Personaje ?origen ?destino - Localizacion)
    :precondition
      (and
        (disponible ?p)
        (en ?p ?origen)
        (caminoCoste3Destruible ?origen ?destino)
      )
    :effect
      (and
        (not (en ?p ?origen))
        (en ?p ?destino)

        ; Se destruye el camino en ambos sentidos.
        (not (caminoCoste3Destruible ?origen ?destino))
        (not (caminoCoste3Destruible ?destino ?origen))

        (increase (total-cost) 3)
      )
  )

  ; Variante de Viajar para caminos de coste 5
  (:action ViajarCoste5
    :parameters (?p - Personaje ?origen ?destino - Localizacion)
    :precondition
      (and
        (disponible ?p)
        (en ?p ?origen)
        (caminoCoste5 ?origen ?destino)
      )
    :effect
      (and
        (not (en ?p ?origen))
        (en ?p ?destino)
        (increase (total-cost) 5)
      )
  )

  ; Variante de Viajar para caminos de coste 8
  (:action ViajarCoste8
    :parameters (?p - Personaje ?origen ?destino - Localizacion)
    :precondition
      (and
        (disponible ?p)
        (en ?p ?origen)
        (caminoCoste8 ?origen ?destino)
      )
    :effect
      (and
        (not (en ?p ?origen))
        (en ?p ?destino)
        (increase (total-cost) 8)
      )
  )

  ; Accion ExtraerRecurso
  ; Parametros: Personaje, localizacion del recurso, tipo de recurso.
  ;
  ; Se conserva la semantica del Ejercicio 1. La accion tiene coste unitario,
  ; ya que el cambio de costes del enunciado afecta especificamente a los
  ; desplazamientos entre localizaciones.
  (:action ExtraerRecurso
    :parameters (?p - Personaje ?l - Localizacion ?r - Recurso)
    :precondition
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
    :effect
      (and
        (trabajando ?p ?l ?r)
        (generando ?r)
        (not (disponible ?p))
      )
  )
)