; Practica 3 - TSI
; Ejercicio 3: se amplía la Comunidad y observamos sus efectos
;
; Autor: Jaime Corzo Galdó

(define (domain tierra-media-ej3a) ; Cabecera que indica el nombre del dominio
  ; Requisitos y capacidades que debe cumplir el planificador para resolver problemas con el dominio
  ; strips: logica de que las acciones solo pueden cambiar el mundo añadiendo cosas nuevas y borrando cosas que ya existían
  ; typing: para usar el guión '-' para asignar tipos a los objetos
  ; existential-preconditions: para usar el comando exists dentro de las condiciones de las acciones
  ; negative-preconditions: Para poder usar el operador not dentro de las precondiciones
  ; disjunctive-preconditions: Para usar el bloque or en la acción de recoger objetos
  ; conditional-effects: para meter el bloque when para destrur el camino de Moria cuando se transita
  (:requirements
    :strips
    :typing
    :negative-preconditions
    :disjunctive-preconditions
    :existential-preconditions
    :conditional-effects
  )

  ; TipoPersonaje se mantiene para representar Enano, Hobbit y Mago como
  ; constantes del dominio, no como tipos PDDL. Asi todos los personajes
  ; concretos siguen siendo objetos de tipo Personaje.
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
  )

  ; Accion Viajar
  ; Parametros: Personaje, Localizacion origen, Localizacion destino.
  ; Permite viajar individualmente solo a personajes disponibles que no sean
  ; miembros de la Comunidad. Para ello se usa una precondicion negativa
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
        ; Si se transita un camino marcado como destruible, se eliminan sus dos
        ; sentidos
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
  ; su tipo pueda extraer. Despues deja de estar disponible.
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

  ; Accion formarComunidad
  ; Parametros: Hobbit, Mago y localizacion en la que se forma la Comunidad.
  ; La seleccion concreta de personajes y punto de reunion no se fuerza en el
  ; codigo: queda en manos del planificador
  (:action formarComunidad
    :parameters (?h - Personaje ?m - Personaje ?l - Localizacion)
    :precondition
      (and
        (not (comunidadFormada))
        (disponible ?h)
        (disponible ?m)
        (personajeEs ?h Hobbit)
        (personajeEs ?m Mago)
        (en ?h ?l)
        (en ?m ?l)
      )
    :effect
      (and
        (comunidadFormada)
        (miembroComunidad ?h)
        (miembroComunidad ?m)
        (comunidadEn ?l)
        ; Al formar la Comunidad, sus miembros dejan de actuar como personajes
        ; individuales. A partir de aqui se desplazan mediante viajarComunidad.
        (not (disponible ?h))
        (not (disponible ?m))
      )
  )

  ; Accion viajarComunidad
  ; Parametros: todos los miembros de la Comunidad, origen y destino.
  ; En este ejercicio la Comunidad reducida esta formada por un Hobbit y un Mago
  (:action viajarComunidad
    :parameters (?h - Personaje ?m - Personaje ?origen ?destino - Localizacion)
    :precondition
      (and
        (comunidadFormada)
        (miembroComunidad ?h)
        (miembroComunidad ?m)
        (personajeEs ?h Hobbit)
        (personajeEs ?m Mago)
        (comunidadEn ?origen)
        (camino ?origen ?destino)
      )
    :effect
      (and
        (not (comunidadEn ?origen))
        (comunidadEn ?destino)
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
  ; Parametros: personaje que recoge, localizacion y objeto.
  ; El Anillo solo puede recogerlo un Hobbit de la Comunidad. El Chaleco y la
  ; Espada solo pueden ser recogidos por el mismo portador del Anillo
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
            (esChaleco ?o)
            (chalecoMaterializado)
            (portadorAnillo ?p)
          )
          (and
            (esEspada ?o)
            (portadorAnillo ?p)
          )
        )
      )
    :effect
      (and
        (tieneObjeto ?p ?o)
        (not (objetoEn ?o ?l))
        (when (esAnillo ?o)
          (and
            (anilloRecogido)
            (portadorAnillo ?p)
          )
        )
      )
  )

  ; Accion destruirAnillo
  ; Parametros: personaje que destruye el Anillo y localizacion.
  ; El lugar de destruccion se indica en el problema mediante lugarDestruccion
  (:action destruirAnillo
    :parameters (?p - Personaje ?l - Localizacion)
    :precondition
      (and
        (comunidadFormada)
        (miembroComunidad ?p)
        (portadorAnillo ?p)
        (personajeEs ?p Hobbit)
        (comunidadEn ?l)
        (lugarDestruccion ?l)
        (tieneObjeto ?p Anillo)
        (tieneObjeto ?p ChalecoMithril)
        (tieneObjeto ?p Espada)
      )
    :effect
      (and
        (anilloDestruido)
      )
  )
)