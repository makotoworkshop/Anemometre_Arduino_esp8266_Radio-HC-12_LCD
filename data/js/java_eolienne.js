      google.charts.load('current', {'packages':['gauge']});
      google.charts.setOnLoadCallback(drawChart);

      function drawChart() {
        // Create and populate the data table.
        var dataGaugeEol = google.visualization.arrayToDataTable([
          ['Label', 'Value'],
          ['Eolienne', 0],
        ]);
        // formattage avec un suffixe et # pour arrondir
        var formatter = new google.visualization.NumberFormat({suffix: ' rpm',pattern:'#'});

        // définition des options documentée ici https://developers.google.com/chart/interactive/docs/gallery/gauge
        var options = {
          width: 800, height: 720,
          greenFrom:300, greenTo: 2200,   // zones de couleurs
          yellowFrom:2200, yellowTo: 2750,
          redFrom: 2750, redTo: 3000,
          majorTicks: ['0', 500, 0, 1500, 0, 2500, 3000],    // graduations
          minorTicks: 5,    // graduations
          max: 3000,
          min: 0,
          animation:{
             duration: 100,
            easing: 'linear',
          }
        };

        // init du graphisme
        var GaugeEol = new google.visualization.Gauge(document.getElementById('chart_div_eolienne'));

        GaugeEol.draw(dataGaugeEol, options);  // tracé du graphisme

up();

        // fonction pour simuler l'aiguille qui bouge
        setInterval(up , 100);    // vitesse de rafraichissement

function up () {

        $.getJSON('/mesures.json',    // avec / pour le nodemcu
 //       $.getJSON("mesures.json",    // sans le / pour ordi local
        function(data){
            $.each(data, 

            function(i, field){

             dataGaugeEol.setValue(0, 1, field);  // premier nb pour le N° de la gauge si plusieurs, puis 0 pour modifier le champ label (sinon1), puis la valeur
             GaugeEol.draw(dataGaugeEol, options);   // tracé du graphisme
             formatter.format(dataGaugeEol,1);  // applique le formattage à la seconde collonne du tableau (1) qui correspond à la valeur
});
});
}
}
