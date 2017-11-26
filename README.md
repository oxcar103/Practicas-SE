Practicas-SE  
========================

Prácticas de la asignatura Sistemas Empotrados (SE).  
------------------------

- **P1-Conexión y gestión remota de la plataforma**: En esta práctica, veremos cómo consultar los manuales para encontrar componentes en la placa, y cómo accederlos y grabarle una imagen en RAM a través de *openocd*.  

- **P2-Introducción al ensamblador**: En esta práctica, tendremos nuestra primera toma de contacto con la arquitectura ARM y su repertorio de instrucciones mediante la configuración de un programa básico en el cuál el led que parpadea depende del último botón pulsado.  

- **P3-Introducción al enlazador y el resto de las binutils**: En esta práctica, configuraremos el script de enlazado, estableciendo las direcciones de memoria que queremos usar y redistribuyendo las zonas de código en ella a nuestro gusto, y definiendo algunas constantes para el acceso rápido a las direcciones del GPIO. Además, modificaremos nuestro programa principal para que se aproveche de estos cambios.  

- **P4-Arranque e inicialización del entorno de ejecución**: En esta práctica, crearemos el *C RunTime* (**CRT**) con una configuración básica: inicializar los dispositivos del sistema(entrada, salida, excepciones,...), establecer los modos de ejecución de nuestra placa, las pilas necesarias y el heap. Además, pasaremos nuestro programa de prueba a C para comprobar que nuestra configuración funciona.  

- **P5-Gestión de excepciones e interrupciones**: En esta práctica, daremos nuestros primeros pasos en el manejo de excepciones así como en la deshabilitación/habilitación de interrupciones. Para ello, cambiaremos un manejador de excepciones y provocaremos dicha excepción; y haremos uso de una poderosa herramienta, el ensamblador en línea, para el cambio de los bits I y F directamente desde C e intentar así construir una región crítica.  
- **P6-El controlador de interrupciones**: En esta práctica, trabajaremos en el control de interrupciones modificando directamente los registros de la Econotag para ello, así como desarrollar más modos de deshabilitación/habilitacin de interrupciones. Para ello, cambiaremos un manejador de interrupciones, y habilitaremos y provocaremos dicha interrupción.  

Licencia  
------------------------
El proyecto en conjunto se distribuye bajo la licencia [GPL](https://github.com/oxcar103/Practicas-SE/blob/master/LICENSE).  

Sin embargo, la licencia no se aplicará sobre el material aportado por el profesor.  

