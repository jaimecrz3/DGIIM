; Practica 3 - TSI
; Ejercicio 3: se amplía la Comunidad y observamos sus efectos
;
; Autor: Jaime Corzo Galdó

(define (domain tierra-media-ej3d) ; Cabecera que indica el nombre del dominio
  ; Requisitos y capacidades que debe cumplir el planificador para resolver problemas con el dominio
  ; strips: logica de que las acciones solo pueden cambiar el mundo añadiendo cosas nuevas y borrando cosas que ya existían
  ; typing: para usar el guión '-' para asignar tipos a los objetos
  ; existential-preconditions: para usar el comando exists dentro de las condiciones de las acciones
  ; negative-preconditions: Para poder usar el operador not dentro de las precondiciones
  ; disjunctive-preconditions: Para usar el
  ; bloque or en la acción de recoger objetos
  ; conditional-effects: para meter el bloque when para destrur el camino de Moria cuando se transita
  ; equality: Para usar el operador de igualdad (=) y desigualdad (not (= ...))
  (:requirements
    :strips
    :typing
    :negative-preconditions
    :disjunctive-preconditions
    :existential-preconditions
    :conditional-effects
    :equality
  )

  ; TipoPersonaje se mantiene para representar Enano, Hobbit y Mago como
  ; constantes del dominio, no como tipos PDDL.
  ; Asi todos los personajes concretos siguen siendo objetos de tipo Personaje.
  (:types
    Personaje
    Localizacion
    Recurso
    TipoPersonaje
    Objeto
  )

  (:constants
    Enano Hobbit Mago Elfo - TipoPersonaje
    Mineral Mithril Madera Especia Alimento - Recurso
    Anillo ChalecoMithril Espada - Objeto
  )

  (:predicates ; Predicados de la ontología
    ; Localizacion fisica de personajes que actuan individualmente
    (en ?p - Personaje ?l - Localizacion)

    ; Recursos disponibles en una localizacion del mapa
    (recursoEn ?r - Recurso ?l - Localizacion)

    ; Camino dirigido entre localizaciones. En el problema se declaran los dos
    ; sentidos para obtener caminos bidireccionales
    (camino ?origen ?destino - Localizacion)

    ; Camino que se destruye cuando se transita. Se usa para modelar el camino
    ; Rivendell-Moria sin introducir acciones adicionales
    (caminoDestruible ?origen ?destino - Localizacion)

    ; Estado laboral de los personajes y recursos que ya se estan generando
    (trabajando ?p - Personaje ?l - Localizacion ?r - Recurso)

    (generando ?r - Recurso)

    ; Disponibilidad para viajar individualmente o empezar a extraer recursos
    (disponible ?p - Personaje)

    ; Tipo de cada personaje y capacidades de extraccion
    (personajeEs ?p - Personaje ?t - TipoPersonaje)
    (puedeExtraerTipo ?t - TipoPersonaje ?r - Recurso)

    ; Estado de la Comunidad. Usamos precondiciones negativas para impedir que
    ; se cree mas de una Comunidad, en lugar de representar explicitamente el
    ; estado contrario mediante un predicado auxiliar
    (comunidadFormada)
    (miembroComunidad ?p - Personaje)
    (comunidadEn ?l - Localizacion)

    ; Objetos y su estado. Se usan predicados de clase para distinguir Anillo,
    ; Chaleco y Espada dentro de una unica accion recogerObjeto
    (objetoEn ?o - Objeto ?l - Localizacion)
    (tieneObjeto ?p - Personaje ?o - Objeto)
    (esAnillo ?o - Objeto)
    (esChaleco ?o - Objeto)
    (esEspada ?o - Objeto)
    (anilloRecogido)
    (portadorAnillo ?p - Personaje)

    ; El chaleco no existe inicialmente: debe materializarlo un Mago de la
    ; Comunidad en una localizacion donde se este extrayendo Mithril
    (chalecoMaterializado)

    ; Lugar de destruccion del Anillo. Se define en el problema para no dejar
    ; Orodruin hardcodeado en el dominio
    (lugarDestruccion ?l - Localizacion)
    (anilloDestruido)

    ; Para comparar qué ID de personaje va antes que otro:
    (idMenor ?p1 ?p2 - Personaje)

    ; Guarda exactamente que personajes forman la Comunidad. para reducir combinaciones dinamicas innecesarias
    (comunidadFormadaPor ?h1 ?h2 ?h3 ?m ?e - Personaje)
  )

  ; Accion Viajar
  ; Parametros: Personaje, Localizacion origen, Localizacion destino.
  ; Permite viajar individualmente solo a personajes disponibles que no sean
  ; miembros de la Comunidad.
  ; Para ello se usa una precondicion negativa
  (:action Viajar
    :parameters (?p - Personaje ?origen ?destino - Localizacion)
    :precondition
      (and
        (disponible ?p)
        (not (miembroComunidad ?p))
        (en ?p ?origen)
        (camino ?origen ?destino)
      )
    :effect
      (and
        (not (en ?p ?origen))
        (en ?p ?destino)
        (when (caminoDestruible ?origen ?destino)
          (and
            (not (camino ?origen ?destino))
            (not (camino ?destino ?origen))
          )
        )
      )
  )

  ; Accion ExtraerRecurso
  ; Parametros: Personaje, Localizacion del recurso, Tipo de recurso.
  ; Igual que en el ejercicio 1: asigna un personaje disponible a un recurso que
  ; su tipo pueda extraer.
  ; Despues deja de estar disponible
  (:action ExtraerRecurso
    :parameters (?p - Personaje ?l - Localizacion ?r - Recurso)
    :precondition
      (and
        (disponible ?p)
        (en ?p ?l)
        (recursoEn ?r ?l)
        (not (generando ?r))
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


  ; Accion formarComunidad
  ; Parametros: 3 Hobbits, 1 Mago, 1 Elfo y localizacion en la que se forma la Comunidad.
  ; La seleccion concreta de personajes y punto de reunion no se fuerza en el
  ; codigo: queda en manos del planificador.
  ; Se usa idMenor para evitar permutaciones equivalentes de los hobbits.
  (:action formarComunidad
    :parameters (?h1 ?h2 ?h3 ?m ?e - Personaje ?l - Localizacion)
    :precondition
      (and
        (not (comunidadFormada))

        ; Los tres primeros personajes deben ser Hobbits.
        (personajeEs ?h1 Hobbit)
        (personajeEs ?h2 Hobbit)
        (personajeEs ?h3 Hobbit)

        ; El cuarto personaje debe ser Mago y el quinto Elfo.
        (personajeEs ?m Mago)
        (personajeEs ?e Elfo)

        ; Deben estar todos disponibles antes de formar la Comunidad.
        (disponible ?h1)
        (disponible ?h2)
        (disponible ?h3)
        (disponible ?m)
        (disponible ?e)

        ; Deben estar todos reunidos en la misma localizacion.
        (en ?h1 ?l)
        (en ?h2 ?l)
        (en ?h3 ?l)
        (en ?m ?l)
        (en ?e ?l)

        ; Los hobbits deben ser distintos. Ademas, se impone un orden
        ; para romper simetrias entre permutaciones equivalentes.
        (idMenor ?h1 ?h2)
        (idMenor ?h2 ?h3)
      )
    :effect
      (and
        (comunidadFormada)

        ; Guardamos la tupla exacta de personajes elegida.
        (comunidadFormadaPor ?h1 ?h2 ?h3 ?m ?e)

        ; Marcamos explicitamente que estos personajes son miembros.
        (miembroComunidad ?h1)
        (miembroComunidad ?h2)
        (miembroComunidad ?h3)
        (miembroComunidad ?m)
        (miembroComunidad ?e)

        ; A partir de este momento se usa una unica localizacion global
        ; para la Comunidad, ya que todos se desplazan conjuntamente.
        (comunidadEn ?l)

        ; Eliminamos su posicion individual para evitar inconsistencias:
        ; desde ahora no viajan por separado.
        (not (en ?h1 ?l))
        (not (en ?h2 ?l))
        (not (en ?h3 ?l))
        (not (en ?m ?l))
        (not (en ?e ?l))
      )
  )


  ; Accion viajarComunidad
  ; Parametros: 3 Hobbits, 1 Mago, 1 Elfo, localizacion origen y localizacion destino.
  ; Aunque internamente se usa comunidadEn para evitar duplicar posiciones,
  ; se mantienen todos los personajes como parametros para respetar el guion.
  (:action viajarComunidad
    :parameters (?h1 ?h2 ?h3 ?m ?e - Personaje ?origen ?destino - Localizacion)
    :precondition
      (and
        (comunidadFormada)

        ; La Comunidad que viaja debe ser exactamente la que se formo.
        ; Esto evita que el planner pruebe combinaciones distintas de miembros
        ; en cada desplazamiento.
        (comunidadFormadaPor ?h1 ?h2 ?h3 ?m ?e)

        ; Se comprueba que los parametros corresponden a la composicion del apartado 3d.
        (personajeEs ?h1 Hobbit)
        (personajeEs ?h2 Hobbit)
        (personajeEs ?h3 Hobbit)
        (personajeEs ?m Mago)
        (personajeEs ?e Elfo)

        ; Mantenemos la ruptura de simetria tambien aqui para no permitir
        ; que la misma Comunidad viaje con permutaciones distintas.
        (idMenor ?h1 ?h2)
        (idMenor ?h2 ?h3)

        ; Movimiento conjunto de la Comunidad.
        (comunidadEn ?origen)
        (camino ?origen ?destino)
      )
    :effect
      (and
        (not (comunidadEn ?origen))
        (comunidadEn ?destino)

        ; Si se cruza el camino destruible Rivendell-Moria o Moria-Rivendell,
        ; se eliminan ambos sentidos del camino.
        (when (caminoDestruible ?origen ?destino)
          (and
            (not (camino ?origen ?destino))
            (not (camino ?destino ?origen))
          )
        )
      )
  )


  ; Accion materializarChaleco
  ; Parametros: Mago que lo crea y localizacion.
  ; El Mago debe pertenecer a la Comunidad y esta debe estar en una localizacion
  ; donde ya haya un personaje extrayendo Mithril
  (:action materializarChaleco
    :parameters (?m - Personaje ?l - Localizacion)
    :precondition
      (and
        (not (chalecoMaterializado))
        (comunidadFormada)
        (miembroComunidad ?m)
        (personajeEs ?m Mago)
        (comunidadEn ?l)
        (exists (?p - Personaje)
          (trabajando ?p ?l Mithril)
        )
      )
    :effect
      (and
        (chalecoMaterializado)
        (objetoEn ChalecoMithril ?l)
      )
  )

  ; Accion recogerObjeto
  ; El portador del anillo se valida directamente consultando el inventario físico (tieneObjeto ?p Anillo).
  (:action recogerObjeto
    :parameters (?p - Personaje ?l - Localizacion ?o - Objeto)
    :precondition
      (and
        (comunidadFormada)
        (miembroComunidad ?p)
        (comunidadEn ?l)
        (objetoEn ?o ?l)
        (or
          (and
            (esAnillo ?o)
            (personajeEs ?p Hobbit)
          )
          (and
            (not (esAnillo ?o))
            (tieneObjeto ?p Anillo)
          )
        )
      )
    :effect
      (and
        (tieneObjeto ?p ?o)
        (not (objetoEn ?o ?l))
      )
  )

  ; Accion destruirAnillo
  ; Al comprobar que el personaje posee físicamente los tres objetos en su inventario, la heurística deduce de forma directa
  ; la validez del estado sin necesidad de evaluar axiomas adicionales
  (:action destruirAnillo
    :parameters (?p - Personaje ?l - Localizacion)
    :precondition
      (and
        (comunidadFormada)
        (comunidadEn ?l)
        (lugarDestruccion ?l)
        (tieneObjeto ?p Anillo)
        (tieneObjeto ?p ChalecoMithril)
        (tieneObjeto ?p Espada)
      )
    :effect
      (anilloDestruido)
  )
)