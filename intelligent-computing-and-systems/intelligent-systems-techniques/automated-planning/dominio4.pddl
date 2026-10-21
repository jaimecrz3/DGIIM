; Practica 3 - TSI
; Ejercicio 4: crear Uruk-Hai en Torre de Hechiceria
;
; Autor: Jaime Corzo Galdo

(define (domain tierra-media-ej4) ; Cabecera que indica el nombre del dominio
  ; Requisitos y capacidades que debe cumplir el planificador para resolver problemas con el dominio
  ; strips: logica de que las acciones solo pueden cambiar el mundo añadiendo cosas nuevas y borrando cosas que ya existían
  ; typing: para usar el guión '-' para asignar tipos a los objetos
  ; existential-preconditions: para usar el comando exists dentro de las condiciones de las acciones
  ; negative-preconditions: Para poder usar el operador not dentro de las precondiciones
  ; disjunctive-preconditions: Para usar el bloque or en la acción de recoger objetos
  ; conditional-effects: para meter el bloque when para destrur el camino de Moria cuando se transita
  ; universal-preconditions: Para poder usar el cuantificador forall
  ; adl: incluye distintos requerimentos, pero en concreto lo usamos para el imply
  (:requirements
    :strips
    :typing
    :negative-preconditions
    :disjunctive-preconditions
    :existential-preconditions
    :universal-preconditions
    :conditional-effects
    :adl
  )

  ; TipoPersonaje representa las especies como constantes, no como
  ; tipos PDDL, ya que todos los individuos concretos son objetos de tipo Personaje
  ;
  ; TipoEdificio se introduce para representar la Torre de Hechiceria y el
  ; Extractor como constantes globales, y así cualquier acción puede hacer referencia a ellos
  (:types
    Personaje
    Localizacion
    Recurso
    TipoPersonaje
    Objeto
    TipoEdificio
  )

  (:constants
    Enano Hobbit Mago Elfo Orco Humano Corsario - TipoPersonaje
    Mineral Mithril Madera Especia Alimento - Recurso
    Anillo ChalecoMithril Espada - Objeto
    TorreHechiceria Extractor - TipoEdificio
  )

  (:predicates
    ; Localizacion fisica de personajes que actuan individualmente.
    (en ?p - Personaje ?l - Localizacion)

    ; Recursos disponibles en una localizacion del mapa.
    (recursoEn ?r - Recurso ?l - Localizacion)

    ; Camino dirigido entre localizaciones. En el problema se declaran ambos
    ; sentidos para representar caminos bidireccionales.
    (camino ?origen ?destino - Localizacion)

    ; Camino que se destruye al ser transitado, usado para Rivendell-Moria.
    (caminoDestruible ?origen ?destino - Localizacion)

    ; Estado laboral de los personajes y recursos que ya se estan generando.
    (trabajando ?p - Personaje ?l - Localizacion ?r - Recurso)
    (generando ?r - Recurso)

    ; Disponibilidad para viajar individualmente o empezar tareas.
    (disponible ?p - Personaje)

    ; Tipo narrativo de cada personaje y capacidades de extraccion.
    (personajeEs ?p - Personaje ?t - TipoPersonaje)
    (puedeExtraerTipo ?t - TipoPersonaje ?r - Recurso)

    ; Estado de la Comunidad.
    (comunidadFormada)
    (miembroComunidad ?p - Personaje)
    (comunidadEn ?l - Localizacion)

    ; Objetos necesarios para destruir el Anillo.
    (objetoEn ?o - Objeto ?l - Localizacion)
    (tieneObjeto ?p - Personaje ?o - Objeto)
    (esAnillo ?o - Objeto)
    (esChaleco ?o - Objeto)
    (esEspada ?o - Objeto)
    (anilloRecogido)
    (portadorAnillo ?p - Personaje)

    ; El chaleco se crea dinamicamente por un Mago de la Comunidad.
    (chalecoMaterializado)

    ; Lugar donde debe destruirse el Anillo.
    (lugarDestruccion ?l - Localizacion)
    (anilloDestruido)


    ; Relacion entre tipos de personaje y edificios que pueden construir,
    ; por ejemplo: Mago construye TorreHechiceria; Humano construye Extractor
    (puedeConstruirTipo ?t - TipoPersonaje ?e - TipoEdificio)

    ; Indica que un edificio puede construirse en una localizacion concreta.
    ; Permite fijar en el problema que la Torre se construye en Isengard y que
    ; el Extractor solo pueda construirse en nodos adecuados de Mineral
    (edificioEnLocalizacionValida ?e - TipoEdificio ?l - Localizacion)

    ; Indica que un edificio ya esta construido en una localizacion.
    ; Sirve tambien para impedir construir dos edificios iguales en el mismo sitio
    (edificioConstruido ?e - TipoEdificio ?l - Localizacion)

    ; Recurso que necesita cada tipo de edificio.
    ; Este predicado es muy importante porque la accion Construir no recibe el recurso
    ; como parametro: lo infiere recorriendo los recursos necesarios
    (recursoNecesario ?e - TipoEdificio ?r - Recurso)

    ; Marca edificios malignos. Para estos edificios se impide que los recursos
    ; necesarios hayan sido extraidos por Enanos
    (edificioMaligno ?e - TipoEdificio)

    ; Algunos recursos, como el Mineral, requieren que exista previamente un Extractor en la localizacion donde se van a extraer
    (requiereExtractor ?r - Recurso)

    ; Estado de la Especia transportada. La Especia, a diferencia de otros
    ; recursos, no queda disponible globalmente para crear el Uruk-Hai: debe
    ; ser recogida y llevada fisicamente hasta la Torre.
    (tieneEspecia ?p - Personaje)

    ; Objetivo asociado a la nueva accion CrearUrukHai.
    (urukHaiCreado)
  )

  ; Accion Viajar
  ; Parametros: Personaje, Localizacion origen, Localizacion destino.
  ; Permite viajar individualmente solo a personajes disponibles que no formen
  ; parte de la Comunidad. Esto permite que orcos, humanos y corsarios se muevan
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
  ;
  ; Se conserva la idea de los ejercicios anteriores, pero se añade una condicion:
  ; si el recurso requiere Extractor, entonces debe existir un Extractor construido
  ; en la misma localizacion. En este problema se usara para el Mineral.
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
        (or ; Si un recurso no tiene la propiedad requiereExtractor, este or permite la acción de extraerRecurso
          (not (requiereExtractor ?r))
          (edificioConstruido Extractor ?l)
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
  ; Parametros: Hobbit, Mago y localizacion donde se forma.
  ; Se mantiene la Comunidad reducida del Ejercicio 3a
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
        (not (disponible ?h))
        (not (disponible ?m))
      )
  )

  ; Accion viajarComunidad
  ; Parametros: Hobbit, Mago, origen y destino.
  ;
  ; Una vez formada, la Comunidad se mueve como una entidad unica mediante
  ; comunidadEn, evitando mantener posiciones individuales redundantes.
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
  ; Parametros: Mago que crea el chaleco y localizacion.
  ;
  ; El Mago debe formar parte de la Comunidad y esta debe estar en una ubicacion
  ; donde se este extrayendo Mithril.
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
  ;
  ; El Anillo solo puede recogerlo un Hobbit de la Comunidad. El Chaleco y la
  ; Espada solo puede recogerlos el portador del Anillo.
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
  ;
  ; El lugar de destruccion se define en el problema con lugarDestruccion.
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

  ; Accion Construir
  ; Parametros: personaje constructor, tipo de edificio y localizacion.
  ;
  ; Esta accion sirve tanto para construir la Torre de Hechiceria como para
  ; construir un Extractor. No recibe el recurso necesario como parametro:
  ; mediante forall recorre todos los recursos y, para aquellos marcados como
  ; necesarios por recursoNecesario, exige mediante exists que ya haya algun
  ; personaje generandolos.
  ;
  ; Ademas, si el edificio es maligno, se exige que esos recursos no hayan sido
  ; extraidos por Enanos.
  ;
  ; La construccion es una excepcion al comportamiento de ExtraerRecurso:
  ; el constructor no deja de estar disponible tras construir. Por ello, por la
  ; hipótesis del mundo cerrado, lo que hacemos es no incluir el (not (disponible ?p)), y de forma
  ; implícita esto quiere decir que sigue disponible
  (:action Construir
    :parameters (?p - Personaje ?e - TipoEdificio ?l - Localizacion)
    :precondition
      (and
        (disponible ?p)
        (not (miembroComunidad ?p))
        (en ?p ?l)
        (edificioEnLocalizacionValida ?e ?l)
        (not (edificioConstruido ?e ?l))

        ; El tipo del personaje debe poder construir el tipo de edificio
        (exists (?t - TipoPersonaje)
          (and
            (personajeEs ?p ?t)
            (puedeConstruirTipo ?t ?e)
          )
        )

        ; Para todo recurso, si ese recurso es necesario para el edificio,
        ; entonces debe existir algun personaje que lo este generando.
        ; Si el edificio es maligno, dicho personaje no puede ser Enano
        ; Con el operador imly, solo se exige la condicion si se cumple que el edificio
        ; requiere el recuros r, en cuyo caso, obliga a que exista un trabajador en estado trabajando
        ; Además, mediante el or, si el edificio es maligno, el trabajador no puede ser enano
        (forall (?r - Recurso)
          (imply
            (recursoNecesario ?e ?r)
            (exists (?trabajador - Personaje ?locRecurso - Localizacion)
              (and
                (trabajando ?trabajador ?locRecurso ?r)
                (or
                  (not (edificioMaligno ?e))
                  (not (personajeEs ?trabajador Enano))
                )
              )
            )
          )
        )
      )
    :effect
      (and
        (edificioConstruido ?e ?l)
      )
  )

  ; Accion RecogerEspecia
  ; Parametros: corsario que recoge la Especia y ubicacion donde la recoge.
  ;
  ; La Especia debe estar siendo extraida en esa ubicacion por algun personaje.
  ; El corsario que la recoge sigue disponible, porque debe transportarla
  ; fisicamente mediante Viajar hasta la Torre de Hechiceria.
  (:action RecogerEspecia
    :parameters (?c - Personaje ?l - Localizacion)
    :precondition
      (and
        (disponible ?c)
        (personajeEs ?c Corsario)
        (en ?c ?l)
        (recursoEn Especia ?l)
        (exists (?otro - Personaje)
          (trabajando ?otro ?l Especia)
        )
      )
    :effect
      (and
        (tieneEspecia ?c)
      )
  )

  ; Accion CrearUrukHai
  ; Parametros: Mago creador y localizacion.
  ;
  ; El Mago que crea el Uruk-Hai no puede pertenecer a la Comunidad. Debe estar
  ; en una localizacion con Torre de Hechiceria construida, y la Especia debe
  ; haber sido transportada fisicamente hasta esa misma localizacion por un
  ; corsario.
  (:action CrearUrukHai
    :parameters (?m - Personaje ?l - Localizacion)
    :precondition
      (and
        (not (urukHaiCreado))
        (disponible ?m)
        (not (miembroComunidad ?m))
        (personajeEs ?m Mago)
        (en ?m ?l)
        (edificioConstruido TorreHechiceria ?l)
        (exists (?c - Personaje)
          (and
            (personajeEs ?c Corsario)
            (tieneEspecia ?c)
            (en ?c ?l)
          )
        )
      )
    :effect
      (and
        (urukHaiCreado)
      )
  )
)