# src/main.cpp
1. seed\_database()
    - Llena la base de datos con información de las particulas y la camara
    - Falta informacion sobre las luces, actualmente creada manualmente

2. create\_camera()
    - Obtiene la camara desde la base de datos
    - Crea struct que es usada en resto del codigo
    - Contine configuracion de la camara y de la ventana de visualizacion

3. create\_particles()
    - Obtiene particulas almacenadas en la base de datos
    - Crea struct que contiene informacion de cada particula

4. create\_light()
    - Crea struct que contine informacion de luz

5. while(camera.window-\>isOpen()
    - Mientras la ventana de la camara esta abierta (filmando?)

6. camera.window-\>clear(sf::Color::Black)
    - Limpia buffer de renderizado usando el color negro

7. poll\_event()
    - Detecta si evento fue generado durante ultimo frame de ejecucion
    - Realiza acciones dependiendo del evento detectado

8. move\_particles()
    - Mueve las particulas segun su posicion y velocidad actuales
    - Si llega a borde de entorno, cambia su velocidad

9. move\_light()
    - Setea la posicion de la luz segun posicion del mouse

10. render\_particles()
    - Renderiza las particulas segun su posicion en relacion a camara e iluminacion
    - Considera distancia a camara para su tamaño
    - Considera distancia a luz para su iluminacion

11. camera.window-\>display()
    - Actualiza lo presentado en ventana segun lo renderizado en buffer de renderizado
