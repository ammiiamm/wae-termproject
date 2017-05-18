// This is a manifest file that'll be compiled into application.js, which will include all the files
// listed below.
//
// Any JavaScript/Coffee file within this directory, lib/assets/javascripts, vendor/assets/javascripts,
// or any plugin's vendor/assets/javascripts directory can be referenced here using a relative path.
//
// It's not advisable to add code directly here, but if you do, it'll appear at the bottom of the
// compiled file.
//
// Read Sprockets README (https://github.com/rails/sprockets#sprockets-directives) for details
// about supported directives.
//
//= require jquery
//= require jquery_ujs
//= require bootstrap-sprockets
//= require Chart.bundle
//= require chartkick
// = require_tree .

function drawgraph(id)
{
    gZoneg = id;
    var httpp = new XMLHttpRequest();
    var urll = "/parkinginfo/free?id=" + id;;
    var paramss = ""
    httpp.open("GET", urll, true);
    httpp.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
    httpp.send(paramss);
    httpp.onreadystatechange = function() {//Call a function when the state changes.
        if(httpp.readyState == 4 && httpp.status == 200) {
            var responce = httpp.responseText;
            var parsed = JSON.parse(responce);
            var str = {};
            for (i = 0; i < parsed.length; i++) {
                str[parsed[i].updated_at] = parsed[i].free;
            }
            data = [
                {name: "Free Parking Spaces", data: str},
            ]
            new Chartkick.LineChart("chart-1",data);
        }
    }}
var gZoneg;

function ChangeDash(id,elements,zoneId) {
    gZoneg = zoneId;
    for (var i = 0; i < elements; i++) {
        if (id == (i+1))
        {
            document.getElementById((i+1)).className = "active";
            var httpp = new XMLHttpRequest();
            var urll = "/parkinginfo/free?id=" + zoneId;;
            var paramss = ""
            httpp.open("GET", urll, true);
            httpp.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
            httpp.send(params);
            httpp.onreadystatechange = function() {//Call a function when the state changes.
                if(httpp.readyState == 4 && httpp.status == 200) {
                    var responce = httpp.responseText;
                    var parsed = JSON.parse(responce);
                    var str = {};
                    for (i = 0; i < parsed.length; i++) {
                        str[parsed[i].updated_at] = parsed[i].free;
                    }
                    data = [
                        {name: "Free Parking Spaces", data: str},
                    ]
                    new Chartkick.LineChart("chart-1",data);
                }
            }
        }
        else
        {
            if (elements>1) {
                document.getElementById((i + 1)).className = "deactivate";
            }
        }
    }
    var http = new XMLHttpRequest();
    var url = "/dashboard/getInfo";
    var params = "id="+id;
    http.open("POST", url, true);
    http.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
    http.send(params);
    http.onreadystatechange = function() {//Call a function when the state changes.
        if(http.readyState == 4 && http.status == 200) {
            var responce = http.responseText;
            document.getElementById("ConfigName").innerHTML = responce;
            document.getElementById("videoMain").src = "http://127.0.0.1:8081/"+id+".ogg";
            var video = document.getElementById('vidMain');
            console.log("test");
            video.load();
        }
    }

}


function runFunctionn()
{
    var httpp = new XMLHttpRequest();
    var urll = "/parkinginfo/free?id=" + gZoneg;
    var paramss = ""
    httpp.open("GET", urll, true);
    httpp.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
    httpp.send(paramss);
    httpp.onreadystatechange = function() {//Call a function when the state changes.
        if(httpp.readyState == 4 && httpp.status == 200) {
            var responce = httpp.responseText;
            var parsed = JSON.parse(responce);
            var str = {};
            for (i = 0; i < parsed.length; i++) {
                str[parsed[i].updated_at] = parsed[i].free;
            }
            data = [
                {name: "Free Parking Spaces", data: str},
            ]
            new Chartkick.LineChart("chart-1",data);
        }
    }}

function mainRunSelector(name)
{
    imglink = '/'+name+'.jpg';
    mainSelector(imglink)
}
var gZone_id;
function globalZone(id)
{
    gZone_id = id;
}
function mainSelector(link)
{
    window.addEventListener("keydown", checkKeyPressed, false);
    document.getElementById("fillBox").addEventListener("click", fillBox);
    document.getElementById("ClearSelection").addEventListener("click", ClearSelection);
    document.getElementById("RemoveLastClick").addEventListener("click", RemoveLastClick);
    document.getElementById("ConfirmSelection").addEventListener("click", ConfirmSelection);
    document.getElementById("download").addEventListener("click", download);
    document.getElementById("CalcHomo").addEventListener("click", CalcHomo);
    document.getElementById("AddLeft").addEventListener("click", AddLeft);
    document.getElementById("AddRight").addEventListener("click", AddRight);
    document.getElementById("easyslot").addEventListener("click", easyslot);
    var homography;
    var HomoCalculated = false;
    paper.setup('myCanvas');
    var raster = new Raster('http://previews.123rf.com/images/lbarn/lbarn0905/lbarn090500018/4906925-large-numbered-space-parking-lot-from-above-Stock-Photo-car.jpg');
    raster.position = view.center;
    var NumOfclick = 0;
    var SelectorArray = [];
    var FinalArray = [];
    // Create a simple drawing tool:
    var tool = new Tool();
    var SelectorPath = new paper.Path();
    SelectorPath.strokeColor = 'red';
    SelectorPath.strokeWidth = 4;
    var SelectedLocations = [];
    var TotalNumberOfElements = 0;
    var xmlString = "";
    var filled = false;

    function GroundToImage(pointx,pointy){
        var InverseHomo = math.zeros(3, 3);
        InverseHomo = math.inv(homography);
        var ImagePoints;
        var Point = math.zeros(3, 1);
        Point._data[0][0] = 10;
        Point._data[1][0] = 15;
        Point._data[2][0] = 1;
        ImagePoints = math.multiply(InverseHomo, Point);
        ImagePoints._data[0][0] = ImagePoints._data[0][0] / ImagePoints._data[2][0];
        ImagePoints._data[1][0] = ImagePoints._data[1][0] / ImagePoints._data[2][0];
        return ImagePoints;
    }

    function ImageToGround(pointx,pointy){
        var GroundPoint;
        var Point = math.zeros(3, 1);
        Point._data[0][0] = pointx;
        Point._data[1][0] = pointy;
        Point._data[2][0] = 1;
        GroundPoint = math.multiply(homography, Point);
        GroundPoint._data[0][0] = GroundPoint._data[0][0] / GroundPoint._data[2][0];
        GroundPoint._data[1][0] = GroundPoint._data[1][0] / GroundPoint._data[2][0];
        return GroundPoint;
    }

    function checkKeyPressed(key) {
        //var x = new Point(0, 0);
        if (key.keyCode == '37')
        {
            // SelectorArray[0].x = SelectorArray[0].x - 1;
            // SelectorPath.removeSegment(0);
            // SelectorPath.insert(0, new Point(SelectorArray[0].x ,SelectorArray[0].y ));
            SelectorArray[1].x = SelectorArray[1].x - 1;
            SelectorPath.removeSegment(1);
            SelectorPath.insert(1, new Point(SelectorArray[1].x ,SelectorArray[1].y ));
            SelectorArray[2].x = SelectorArray[2].x - 1;
            SelectorPath.removeSegment(2);
            SelectorPath.insert(2, new Point(SelectorArray[2].x ,SelectorArray[2].y ));
            SelectorArray[3].x = SelectorArray[3].x - 1;
            SelectorPath.removeSegment(3);
            SelectorPath.insert(3, new Point(SelectorArray[3].x ,SelectorArray[3].y ));
            if (filled) {
                SelectorArray[4].x = SelectorArray[4].x - 1;
                SelectorPath.removeSegment(4);
                SelectorPath.insert(4, new Point(SelectorArray[4].x, SelectorArray[4].y));
            }
            var ground;
            var image;
            ground = ImageToGround(SelectorArray[0].x,SelectorArray[0].y);
            console.log(ground._data[0][0]);
            ground._data[0][0] = ground._data[0][0] - 8;
            console.log(ground._data[0][0]);
            image = GroundToImage(ground._data[0][0] ,ground._data[1][0]);
            SelectorPath.removeSegment(0);
            SelectorPath.insert(0, new Point(image._data[0][0] ,image._data[1][0]));
            SelectorArray[0].x = image._data[0][0];
            SelectorArray[0].y = image._data[1][0];
            // console.log(SelectorArray[0])
        }
        if (key.keyCode == '40')
        {
            SelectorArray[0].y = SelectorArray[0].y + 1;
            SelectorPath.removeSegment(0);
            SelectorPath.insert(0, new Point(SelectorArray[0].x ,SelectorArray[0].y ));
            SelectorArray[1].y = SelectorArray[1].y + 1;
            SelectorPath.removeSegment(1);
            SelectorPath.insert(1, new Point(SelectorArray[1].x ,SelectorArray[1].y ));
            SelectorArray[2].y = SelectorArray[2].y + 1;
            SelectorPath.removeSegment(2);
            SelectorPath.insert(2, new Point(SelectorArray[2].x ,SelectorArray[2].y ));
            SelectorArray[3].y = SelectorArray[3].y + 1;
            SelectorPath.removeSegment(3);
            SelectorPath.insert(3, new Point(SelectorArray[3].x ,SelectorArray[3].y ));
            if (filled) {
                SelectorArray[4].y = SelectorArray[4].y + 1;
                SelectorPath.removeSegment(4);
                SelectorPath.insert(4, new Point(SelectorArray[4].x, SelectorArray[4].y));
            }
        }
        if (key.keyCode == '39')
        {
            SelectorArray[0].x = SelectorArray[0].x + 1;
            SelectorPath.removeSegment(0);
            SelectorPath.insert(0, new Point(SelectorArray[0].x ,SelectorArray[0].y ));
            SelectorArray[1].x = SelectorArray[1].x + 1;
            SelectorPath.removeSegment(1);
            SelectorPath.insert(1, new Point(SelectorArray[1].x ,SelectorArray[1].y ));
            SelectorArray[2].x = SelectorArray[2].x + 1;
            SelectorPath.removeSegment(2);
            SelectorPath.insert(2, new Point(SelectorArray[2].x ,SelectorArray[2].y ));
            SelectorArray[3].x = SelectorArray[3].x + 1;
            SelectorPath.removeSegment(3);
            SelectorPath.insert(3, new Point(SelectorArray[3].x ,SelectorArray[3].y ));
            if (filled) {
                SelectorArray[4].x = SelectorArray[4].x + 1;
                SelectorPath.removeSegment(4);
                SelectorPath.insert(4, new Point(SelectorArray[4].x, SelectorArray[4].y));
            }
        }
        if (key.keyCode == '38')
        {
            SelectorArray[0].y = SelectorArray[0].y - 1;
            SelectorPath.removeSegment(0);
            SelectorPath.insert(0, new Point(SelectorArray[0].x ,SelectorArray[0].y ));
            SelectorArray[1].y = SelectorArray[1].y - 1;
            SelectorPath.removeSegment(1);
            SelectorPath.insert(1, new Point(SelectorArray[1].x ,SelectorArray[1].y ));
            SelectorArray[2].y = SelectorArray[2].y - 1;
            SelectorPath.removeSegment(2);
            SelectorPath.insert(2, new Point(SelectorArray[2].x ,SelectorArray[2].y ));
            SelectorArray[3].y = SelectorArray[3].y - 1;
            SelectorPath.removeSegment(3);
            SelectorPath.insert(3, new Point(SelectorArray[3].x ,SelectorArray[3].y ));
            if (filled) {
                SelectorArray[4].y = SelectorArray[4].y - 1;
                SelectorPath.removeSegment(4);
                SelectorPath.insert(4, new Point(SelectorArray[4].x, SelectorArray[4].y));
            }
        }
        paper.view.draw();
    }

    tool.onMouseDown = function(event) {
        document.getElementById("dataWidth").placeholder = event.point;
        if (NumOfclick == 0)
        {
            SelectorPath.moveTo(event.point);
            SelectorArray.push(event.point);
        }
        else {
            SelectorPath.lineTo(event.point);
            SelectorArray.push(event.point);
        }
        paper.view.draw();
        NumOfclick++;
        document.getElementById("dataHeight").placeholder = NumOfclick;
    };
    tool.onMouseMove = function(event) {
        document.getElementById("dataX").placeholder = event.point.x;
        document.getElementById("dataY").placeholder = event.point.y;
    };
    function fillBox() {
        SelectorPath.lineTo(SelectorArray[0]);
        SelectorArray.push(SelectorArray[0]);
        paper.view.draw();
        NumOfclick++;
        filled = true;
    }
    function ClearSelection() {
        SelectorPath.clear();
        SelectorArray = [];
        NumOfclick = 0;
        paper.view.draw();
        document.getElementById("dataHeight").placeholder = NumOfclick;
    }
    function RemoveLastClick() {
        SelectorArray.splice((NumOfclick-1),1);
        SelectorPath.removeSegment((NumOfclick-1));
        NumOfclick--;
        paper.view.draw();
        document.getElementById("dataHeight").placeholder = NumOfclick;
    }
    function ConfirmSelection() {
        FinalArray.push(SelectorArray);
        SelectorPath.clear();
        SelectorArray = [];
        NumOfclick = 0;
        paper.view.draw();
        document.getElementById("dataHeight").placeholder = NumOfclick;
        filled = false;
        drawSelectedItems();
    }
    function drawSelectedItems()
    {
        SelectedLocations[TotalNumberOfElements] = new paper.Path();
        SelectedLocations[TotalNumberOfElements].strokeColor = 'green';
        SelectedLocations[TotalNumberOfElements].strokeWidth = 4;
        for (var j = 0; j < 4; j++) {
            SelectedLocations[TotalNumberOfElements].lineTo(FinalArray[TotalNumberOfElements][j]);
        }
        SelectedLocations[TotalNumberOfElements].lineTo(FinalArray[TotalNumberOfElements][0]);
        TotalNumberOfElements ++;
        document.getElementById("totalSelected").placeholder = TotalNumberOfElements;
    }
    function generateXml(){
        xmlString += "<?xml version=\"1.0\"  encoding=\"UTF-8\" ?>\n";
        xmlString += "<xml>\n";
        for (var i = 0; i < TotalNumberOfElements; i++) {
            xmlString += "<Slot>\n";
            for (var j = 0; j < 4; j++) {
                xmlString += "<Point>\n";
                xmlString += "<x>\n";
                xmlString += FinalArray[i][j].x;
                xmlString += "</x>\n";
                xmlString += "<y>\n";
                xmlString += FinalArray[i][j].y;
                xmlString += "</y>\n";
                xmlString += "</Point>\n";
            }
            xmlString += "</Slot>\n";
        }
        xmlString += "</xml>\n";
    }
    function download() {
        generateXml();
        var textToSave = xmlString;
        var textToSaveAsBlob = new Blob([textToSave], {type:"text/xml"});
        var textToSaveAsURL = window.URL.createObjectURL(textToSaveAsBlob);
        var fileNameToSaveAs = "ParkingLotConfig";

        var downloadLink = document.createElement("a");
        downloadLink.download = fileNameToSaveAs;
        downloadLink.innerHTML = "Download File";
        downloadLink.href = textToSaveAsURL;
        downloadLink.style.display = "none";
        document.body.appendChild(downloadLink);
        downloadLink.click();
        xmlString = "";
    }
    function CalcHomo(){
        var transform = new jsfeat.matrix_t(3, 3, jsfeat.F32_t | jsfeat.C1_t);
        var x0 =  document.getElementById("hx0").value;
        var y0 =  document.getElementById("hy0").value;
        var x1 =  document.getElementById("hx1").value;
        var y1 =  document.getElementById("hy1").value;
        var x2 =  document.getElementById("hx2").value;
        var y2 =  document.getElementById("hy2").value;
        var x3 =  document.getElementById("hx3").value;
        var y3 =  document.getElementById("hy3").value;
        matval = SelectorArray[0].x + "," + SelectorArray[0].y + "," + SelectorArray[1].x + "," + SelectorArray[1].y + "," +
            SelectorArray[2].x + "," + SelectorArray[2].y + "," + SelectorArray[3].x + "," + SelectorArray[3].y + ",";
        realval = matval + x0 + "," + y0 + "," + x1 + "," + y1 + "," + x2 + "," + y2 + "," + x3 + "," + y3;
        var http = new XMLHttpRequest();
        var url = "http://localhost:3000/homography/calchm";
        var params = "task=1&values=" + realval;
        http.open("POST", url, true);
        http.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
        http.send(params);
        http.onreadystatechange = function() {//Call a function when the state changes.
            if(http.readyState == 4 && http.status == 200) {
                var responce = http.responseText;
                homography = responce.split(",");
                console.log(responce);
                document.getElementById("h1").placeholder = homography[0];
                document.getElementById("h2").placeholder = homography[1];
                document.getElementById("h3").placeholder = homography[2];
                document.getElementById("h4").placeholder = homography[3];
                document.getElementById("h5").placeholder = homography[4];
                document.getElementById("h6").placeholder = homography[5];
                document.getElementById("h7").placeholder = homography[6];
                document.getElementById("h8").placeholder = homography[7];
                document.getElementById("h9").placeholder = homography[8];
            }
        };

        HomoCalculated = true;
        SelectorPath.clear();
        SelectorArray = [];
        NumOfclick = 0;
        paper.view.draw();
        document.getElementById("dataHeight").placeholder = NumOfclick;
    }

    function AddLeft(){
        if(!HomoCalculated){ alert("Please Calculate Homography Before Using this Feature"); }
        else {
            var homo = homography[0] + "," + homography[1] + "," + homography[2] + "," + homography[3] + "," +
                homography[4] + "," + homography[5] + "," + homography[6] + "," + homography[7] + "," +
                homography[8] + "," + SelectorArray[0].x + "," + SelectorArray[0].y + "," + SelectorArray[1].x + "," + SelectorArray[1].y + "," + SelectorArray[3].x + "," + SelectorArray[3].y;
            var http = new XMLHttpRequest();
            var url = "http://localhost:3000/homography/calchm";
            var params = "task=2&values=" + homo;
            http.open("POST", url, true);
            http.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
            http.send(params);
            http.onreadystatechange = function() {//Call a function when the state changes.
                if(http.readyState == 4 && http.status == 200) {
                    var responce = http.responseText;
                    var newpoints = responce.split(",");
                    var new1x = newpoints[0];
                    var new1y = newpoints[1];
                    var new2x = newpoints[2];
                    var new2y = newpoints[3];
                    var points = [];
                    points.push({x:SelectorArray[0].x, y:SelectorArray[0].y});
                    points.push({x:SelectorArray[3].x, y:SelectorArray[3].y});
                    points.push({x:new1x, y:new1y});
                    points.push({x:new2x, y:new2y});
                    var temp1 = new Point(SelectorArray[0].x,SelectorArray[0].y);
                    var temp2 = new Point(SelectorArray[3].x,SelectorArray[3].y);
                    console.log(new1x);
                    console.log(new1y);
                    var temp3 = new Point(10,20);
                    console.log(temp3);
                    var temp4 = new Point(new2x,new2y);
                    ConfirmSelection();
                    SelectorPath.moveTo(points[2]);
                    SelectorArray.push(points[2]);
                    SelectorPath.lineTo(points[0]);
                    SelectorArray.push(points[0]);
                    SelectorPath.lineTo(points[1]);
                    SelectorArray.push(points[1]);
                    SelectorPath.lineTo(points[3]);
                    SelectorArray.push(points[3]);
                    paper.view.draw();
                }
            }
        }
    }

    function AddRight(){
        if(!HomoCalculated){ alert("Please Calculate Homography Before Using this Feature"); }
        else {
            var homo = homography[0] + "," + homography[1] + "," + homography[2] + "," + homography[3] + "," +
                homography[4] + "," + homography[5] + "," + homography[6] + "," + homography[7] + "," +
                homography[8] + "," + SelectorArray[1].x + "," + SelectorArray[1].y + "," + SelectorArray[0].x + "," + SelectorArray[0].y + "," + SelectorArray[2].x + "," + SelectorArray[2].y;
            var http = new XMLHttpRequest();
            var url = "http://localhost:3000/homography/calchm";
            var params = "task=3&values=" + homo;
            http.open("POST", url, true);
            http.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
            http.send(params);
            http.onreadystatechange = function() {//Call a function when the state changes.
                if(http.readyState == 4 && http.status == 200) {
                    var responce = http.responseText;
                    var newpoints = responce.split(",");
                    var new1x = newpoints[0];
                    var new1y = newpoints[1];
                    var new2x = newpoints[2];
                    var new2y = newpoints[3];
                    var points = [];
                    points.push({x:SelectorArray[1].x, y:SelectorArray[1].y});
                    points.push({x:SelectorArray[2].x, y:SelectorArray[2].y});
                    points.push({x:new1x, y:new1y});
                    points.push({x:new2x, y:new2y});
                    var temp1 = new Point(SelectorArray[1].x,SelectorArray[1].y);
                    var temp2 = new Point(SelectorArray[2].x,SelectorArray[2].y);
                    var temp3 = new Point(new1x,new1y);
                    var temp4 = new Point(new2x,new2y);
                    ConfirmSelection();
                    SelectorPath.moveTo(points[2]);
                    SelectorArray.push(points[2]);
                    SelectorPath.lineTo(points[0]);
                    SelectorArray.push(points[0]);
                    SelectorPath.lineTo(points[1]);
                    SelectorArray.push(points[1]);
                    SelectorPath.lineTo(points[3]);
                    SelectorArray.push(points[3]);
                    paper.view.draw();
                }
            }
        }
    }
    var first = false;
    function add(pointt)
    {
        if(first)
        {
            SelectorPath.moveTo(pointt);
            SelectorArray.push(pointt);
            first = false;
        }
        else
        {
            SelectorPath.lineTo(pointt);
            SelectorArray.push(pointt);
        }
        paper.view.draw();

    }
    function easyslot()
    {
        // var x0 =  document.getElementById("hx0").value;
        // var y0 =  document.getElementById("hy0").value;
        // var x1 =  document.getElementById("hx1").value;
        // var y1 =  document.getElementById("hy1").value;
        // var x2 =  document.getElementById("hx2").value;
        // var y2 =  document.getElementById("hy2").value;
        // var x3 =  document.getElementById("hx3").value;
        // var y3 =  document.getElementById("hy3").value;
        var divider = parseInt(document.getElementById("numberofslots").value);
        var x0 =  0;
        var y0 =  0;
        var x1 =  100;
        var y1 =  0;
        var x2 =  100;
        var y2 =  30;
        var x3 =  0;
        var y3 =  30;
        matval = SelectorArray[0].x + "," + SelectorArray[0].y + "," + SelectorArray[1].x + "," + SelectorArray[1].y + "," +
            SelectorArray[2].x + "," + SelectorArray[2].y + "," + SelectorArray[3].x + "," + SelectorArray[3].y + ",";
        realval = matval + x0 + "," + y0 + "," + x1 + "," + y1 + "," + x2 + "," + y2 + "," + x3 + "," + y3;
        var http = new XMLHttpRequest();
        var url = "http://localhost:3000/homography/calchm";
        var params = "task=2&values=" + realval + "&divide=" + divider;
        http.open("POST", url, true);
        http.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
        http.send(params);
        http.onreadystatechange = function() {//Call a function when the state changes.
            if(http.readyState == 4 && http.status == 200) {
                var responce = http.responseText;
                homography = responce.split(",");
                var newpoints = responce.split(",");
                var points = [];
                var first = true;
                for (i = 0; i < (newpoints.length-1); i=i+2) {
                    points.push({x:newpoints[i], y:newpoints[i+1]});
                }

                for (i = 0; i < (points.length/2); i++) {
                    var zone_id = gZone_id;
                    var name = 'p';
                    var status = 'false';
                    var point1;
                    var point2;
                    var point3;
                    var point4;
                    var xx = parseInt(points[i].x);
                    var yy = parseInt(points[i].y);
                    var temp3 = new Point(xx,yy);
                    add(temp3);
                    point1 = "("+xx+","+yy+")";
                    var xx = parseInt(points[i+1+divider].x);
                    var yy = parseInt(points[i+1+divider].y);
                    var temp3 = new Point(xx,yy);
                    add(temp3);
                    point2 = "("+xx+","+yy+")";
                    var xx = parseInt(points[i+2+divider].x);
                    var yy = parseInt(points[i+2+divider].y);
                    var temp3 = new Point(xx,yy);
                    add(temp3);
                    point3 = "("+xx+","+yy+")";
                    var xx = parseInt(points[i+1].x);
                    var yy = parseInt(points[i+1].y);
                    var temp3 = new Point(xx,yy);
                    add(temp3);
                    point4 = "("+xx+","+yy+")";
                    var httpp = new XMLHttpRequest();
                    var urll = "/parkinginfo/addslot?zone_id="+zone_id+"&name="+name+"&status="+status+"&point1="+point1+"&point2="+point2+"&point3="+point3+"&point4=" + point4;
                    var params = "";
                    httpp.open("POST", urll, true);
                    httpp.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
                    httpp.send(params);
                    httpp.onreadystatechange = function() {//Call a function when the state changes.
                        if(httpp.readyState == 4 && httpp.status == 200) {
                        }
                    }
                }
                paper.view.draw();
            }
        };
        HomoCalculated = true;
        //SelectorPath.clear();
        //SelectorArray = [];
        NumOfclick = 0;
        // paper.view.draw();
        // document.getElementById("dataHeight").placeholder = NumOfclick;
    }
}