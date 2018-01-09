// the polylines
var poly = new google.maps.Polyline({
  strokeColor: '#0000FF',
  strokeOpacity: 1.0,
  strokeWeight: 1,
  editable: true
});

// the path of polyline 
var path;

var marker_title = '';

// the map
var map;

// unique marker id
var marker_id = 0;

// is delete menu open
var delMenuOpen = false;

// info window 
var infowindow = new google.maps.InfoWindow();

var markers = [];

function initMap() {
  map = new google.maps.Map(document.getElementById('map'), {
    zoom: 7,
    center: {lat: 51.1784, lng: -115.5808},  // Center the map on Banff, AB, Canada.
    mapTypeId: 'terrain'

  });

  poly.setMap(map);

  // listener for creating polyline
  google.maps.event.addListener(map, 'click', function(event) {
    addLatLng(event);

  });
  

  // listenter for setting a marker
  google.maps.event.addListener(map, 'rightclick', function(event) {
    addMarker(event.latLng, map);



  });
  
}

// Deletes all markers from map
function delete_markers() {
  for (var i = 0; i < markers.length; i++) {
    markers[i].setMap(null); 
  }
}

// Removes polyline from map
function delete_poly() {  
  path.clear();  
}


// Handles click events on a map, and adds a new point to the Polyline.
// includes ability to delete vertex points by listening for right clicks on vertexes
function addLatLng(event) {

  var deleteMenu = new DeleteMenu();

  // check for right clicks on vertexes of polylines  
  google.maps.event.addListener(poly, 'rightclick', function(e) {
    // Check if click was on a vertex control point
    if (e.vertex == undefined) {
      return;
    }      
    deleteMenu.open(map, poly.getPath(), e.vertex);
    // bypasses adding new point to polyline when user brings up delete menu via if statement below
    delMenuOpen = true;
    });

  // bypass adding new point to polyline
  if (delMenuOpen) {  
    delMenuOpen = false;
    return;

  }else {
    path = poly.getPath();

    // Because path is an MVCArray, we can simply append a new coordinate and it will automatically appear.
    path.push(event.latLng);

  }

  
}

function printPath() {
  var printThis = "";
  var pathArray = path.getArray();
  for (var i = 0; i < pathArray.length; i++) {
    printThis += pathArray[i] + "; ";
  }
  
  document.getElementById('ll').value = printThis;
}

// https://developers.google.com/maps/documentation/javascript/examples/delete-vertex-menu
/**
* A menu that lets a user delete a selected vertex of a path.
* @constructor
*/
function DeleteMenu() {
  this.div_ = document.createElement('div');
  this.div_.className = 'delete-menu';
  this.div_.innerHTML = 'Delete';

  var menu = this;
  google.maps.event.addDomListener(this.div_, 'click', function() {
    menu.removeVertex();
  });
}

DeleteMenu.prototype = new google.maps.OverlayView();

DeleteMenu.prototype.onAdd = function() {
  var deleteMenu = this;
  var map = this.getMap();
  this.getPanes().floatPane.appendChild(this.div_);

  // mousedown anywhere on the map except on the menu div will close the menu.
  this.divListener_ = google.maps.event.addDomListener(map.getDiv(), 'mousedown', function(e) {
    if (e.target != deleteMenu.div_) {
      deleteMenu.close();
    }
  }, true);
};

DeleteMenu.prototype.onRemove = function() {
  google.maps.event.removeListener(this.divListener_);
  this.div_.parentNode.removeChild(this.div_);

  // clean up
  this.set('position');
  this.set('path');
  this.set('vertex'); 
};

DeleteMenu.prototype.close = function() {
  this.setMap(null);
};

DeleteMenu.prototype.draw = function() {
  var position = this.get('position');
  var projection = this.getProjection();

  if (!position || !projection) {
      return;
  }

  var point = projection.fromLatLngToDivPixel(position);
    this.div_.style.top = point.y + 'px';
    this.div_.style.left = point.x + 'px';
};

/**
 * Opens the menu at a vertex of a given path.
 */
DeleteMenu.prototype.open = function(map, path, vertex) {
  this.set('position', path.getAt(vertex));
  this.set('path', path);
  this.set('vertex', vertex);
  this.setMap(map);
  this.draw();
};
  
/**
 * Deletes the vertex from the path.
 */
DeleteMenu.prototype.removeVertex = function() {
  var path = this.get('path');
  var vertex = this.get('vertex');

  if (!path || vertex == undefined) {
    this.close();
    return;
  }

  path.removeAt(vertex);
  this.close();
};

  
  
function addMarker(location, map) {

  // increment unique id for marker and set marker id 
  marker_id++;
  var m_title = '...';
  
  // set new marker with unique id
  var marker = new google.maps.Marker( {
    position: location,
    draggable: true,
    map: map,
    id: marker_id,
    title: m_title
  });
  
  //input for marker title
  marker_title = '<input type="text"; id="m_title_id"></input></br>';
    
  // create info menu content
  var marker_latlng = 'Lat: ' + marker.position.lat().toFixed(5) + ' | Long: ' + marker.position.lng().toFixed(5) + '<br />';
  
  // create input form for marker description
//  var marker_description_html = '</br><textarea rows="3"; cols="25"; placeholder="Description"; id="marker_description"></textarea></br>';

  // form save button to submit
  var marker_saveButton = '<button type="button"; onclick="marker.setTitle(document.getElementById(\'m_title_id\'))">Save Title</button>';
  
  // create delete button in marker menu
  var marker_deleteButton = '</br><input type="button"; value="Delete"; onclick="deleteMarker(' + marker.id + ')">';

  var menu_content = marker_title + marker_latlng + marker_saveButton + marker_deleteButton;
  
  // bring up info window when user clicks on marker
  marker.addListener('click', function() {
    infowindow.setContent(menu_content);
    infowindow.open(map, marker);
  });
  
  marker.addListener('drag', function() {
    marker_latlng = 'Lat: ' + marker.position.lat().toFixed(5) + ' | Long: ' + marker.position.lng().toFixed(5) + '<br />';
    menu_content = marker_title + marker_latlng + marker_saveButton + marker_deleteButton;
    infowindow.setContent(menu_content);
    infowindow.open(map, marker);    
  });

  marker.addListener('dragend', function() {
    marker_latlng = 'Lat: ' + marker.position.lat().toFixed(5) + ' | Long: ' + marker.position.lng().toFixed(5) + '<br />';
    menu_content = marker_title + marker_latlng+ marker_saveButton + marker_deleteButton;
    infowindow.setContent(menu_content);
    infowindow.open(map, marker);
  });


  // add marker to array
  markers.push(marker);
}



function deleteMarker(id) {
  //Find and remove the marker from the Array
  for (var i = 0; i < markers.length; i++) {
    if (markers[i].id == id) {
      //Remove the marker from Map                  
      markers[i].setMap(null);
 
      //Remove the marker from array.
      markers.splice(i, 1);
      return;
    }
  }
}



//download.js v4.2, by dandavis; 2008-2016. [CCBY2] see http://danml.com/download.html for tests/usage
// v1 landed a FF+Chrome compat way of downloading strings to local un-named files, upgraded to use a hidden frame and optional mime
// v2 added named files via a[download], msSaveBlob, IE (10+) support, and window.URL support for larger+faster saves than dataURLs
// v3 added dataURL and Blob Input, bind-toggle arity, and legacy dataURL fallback was improved with force-download mime and base64 support. 3.1 improved safari handling.
// v4 adds AMD/UMD, commonJS, and plain browser support
// v4.1 adds url download capability via solo URL argument (same domain/CORS only)
// v4.2 adds semantic variable names, long (over 2MB) dataURL support, and hidden by default temp anchors
// https://github.com/rndme/download

(function (root, factory) {
	if (typeof define === 'function' && define.amd) {
		// AMD. Register as an anonymous module.
		define([], factory);
	} else if (typeof exports === 'object') {
		// Node. Does not work with strict CommonJS, but
		// only CommonJS-like environments that support module.exports,
		// like Node.
		module.exports = factory();
	} else {
		// Browser globals (root is window)
		root.download = factory();
  }
}(this, function () {

	return function download(data, strFileName, strMimeType) {

		var self = window, // this script is only for browsers anyway...
			defaultMime = "application/octet-stream", // this default mime also triggers iframe downloads
			mimeType = strMimeType || defaultMime,
			payload = data,
			url = !strFileName && !strMimeType && payload,
			anchor = document.createElement("a"),
			toString = function(a){return String(a);},
			myBlob = (self.Blob || self.MozBlob || self.WebKitBlob || toString),
			fileName = strFileName || "download",
			blob,
			reader;
			myBlob= myBlob.call ? myBlob.bind(self) : Blob ;
	  
		if(String(this)==="true"){ //reverse arguments, allowing download.bind(true, "text/xml", "export.xml") to act as a callback
			payload=[payload, mimeType];
			mimeType=payload[0];
			payload=payload[1];
		}


		if(url && url.length< 2048){ // if no filename and no mime, assume a url was passed as the only argument
			fileName = url.split("/").pop().split("?")[0];
			anchor.href = url; // assign href prop to temp anchor
		  	if(anchor.href.indexOf(url) !== -1){ // if the browser determines that it's a potentially valid url path:
        		var ajax=new XMLHttpRequest();
        		ajax.open( "GET", url, true);
        		ajax.responseType = 'blob';
        		ajax.onload= function(e){ 
				  download(e.target.response, fileName, defaultMime);
				};
        		setTimeout(function(){ ajax.send();}, 0); // allows setting custom ajax headers using the return:
			    return ajax;
			} // end if valid url?
		} // end if url?


		//go ahead and download dataURLs right away
		if(/^data\:[\w+\-]+\/[\w+\-]+[,;]/.test(payload)){
		
			if(payload.length > (1024*1024*1.999) && myBlob !== toString ){
				payload=dataUrlToBlob(payload);
				mimeType=payload.type || defaultMime;
			}else{			
				return navigator.msSaveBlob ?  // IE10 can't do a[download], only Blobs:
					navigator.msSaveBlob(dataUrlToBlob(payload), fileName) :
					saver(payload) ; // everyone else can save dataURLs un-processed
			}
			
		}//end if dataURL passed?

		blob = payload instanceof myBlob ?
			payload :
			new myBlob([payload], {type: mimeType}) ;


		function dataUrlToBlob(strUrl) {
			var parts= strUrl.split(/[:;,]/),
			type= parts[1],
			decoder= parts[2] == "base64" ? atob : decodeURIComponent,
			binData= decoder( parts.pop() ),
			mx= binData.length,
			i= 0,
			uiArr= new Uint8Array(mx);

			for(i;i<mx;++i) uiArr[i]= binData.charCodeAt(i);

			return new myBlob([uiArr], {type: type});
		 }

		function saver(url, winMode){

			if ('download' in anchor) { //html5 A[download]
				anchor.href = url;
				anchor.setAttribute("download", fileName);
				anchor.className = "download-js-link";
				anchor.innerHTML = "downloading...";
				anchor.style.display = "none";
				document.body.appendChild(anchor);
				setTimeout(function() {
					anchor.click();
					document.body.removeChild(anchor);
					if(winMode===true){setTimeout(function(){ self.URL.revokeObjectURL(anchor.href);}, 250 );}
				}, 66);
				return true;
			}

			// handle non-a[download] safari as best we can:
			if(/(Version)\/(\d+)\.(\d+)(?:\.(\d+))?.*Safari\//.test(navigator.userAgent)) {
				url=url.replace(/^data:([\w\/\-\+]+)/, defaultMime);
				if(!window.open(url)){ // popup blocked, offer direct download:
					if(confirm("Displaying New Document\n\nUse Save As... to download, then click back to return to this page.")){ location.href=url; }
				}
				return true;
			}

			//do iframe dataURL download (old ch+FF):
			var f = document.createElement("iframe");
			document.body.appendChild(f);

			if(!winMode){ // force a mime that will download:
				url="data:"+url.replace(/^data:([\w\/\-\+]+)/, defaultMime);
			}
			f.src=url;
			setTimeout(function(){ document.body.removeChild(f); }, 333);

		}//end saver


		if (navigator.msSaveBlob) { // IE10+ : (has Blob, but not a[download] or URL)
			return navigator.msSaveBlob(blob, fileName);
		}

		if(self.URL){ // simple fast and modern way using Blob and URL:
			saver(self.URL.createObjectURL(blob), true);
		}else{
			// handle non-Blob()+non-URL browsers:
			if(typeof blob === "string" || blob.constructor===toString ){
				try{
					return saver( "data:" +  mimeType   + ";base64,"  +  self.btoa(blob)  );
				}catch(y){
					return saver( "data:" +  mimeType   + "," + encodeURIComponent(blob)  );
				}
			}
			// Blob but not URL support:
			reader=new FileReader();
			reader.onload=function(e){
				saver(this.result);
			};
			reader.readAsDataURL(blob);
		}
		return true;
	}; /* end download() */
}));


function createDownloadLink() {

  var str = '<?xml version="1.0" encoding="UTF-8"?><kml xmlns="http://www.opengis.net/kml/2.2"><Document><Style id="bendigo_line"><LineStyle><width>3</width><color>ff33ccff</color></LineStyle></Style><Placemark><styleUrl>#bendigo_line</styleUrl><LineString><coordinates>';
  
  var a_path = poly.getPath();
  var length = a_path.getLength();
  for (var i=0; i < length; i++) {
    str += a_path.getAt(i).lng()+","+a_path.getAt(i).lat()+",0 ";
  }
  
  var end_text = '</coordinates></LineString></Placemark></Document></kml>';

  str += end_text;      

  download(str, "KMLinfo.kml", "text/plain");
}
