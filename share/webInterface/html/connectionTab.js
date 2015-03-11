
/*function calc() {
  var doc = iframe.contentDocument;
  doc.body.style.background = 'red';
//  frm=document.forms[0]
  frm=doc.forms[0]
  url="equal.1?a="+frm.elements['a'].value
  xmlhttp.open("GET",url,true);
  xmlhttp.onreadystatechange=function() {
   if (xmlhttp.readyState==4) {
//    document.forms[0].elements['total'].value=xmlhttp.responseText
    doc.forms[0].elements['total'].value=xmlhttp.responseText
   }
  }
 xmlhttp.setRequestHeader('Accept','message/x-jl-formresult')
 xmlhttp.send()
 return false
}*/

// This iframe stuff is from http://wiki.greasespot.net/CSS_Independent_Content [thanks!]

// position:fixed means stay fixed even when the page scrolls. z-index keeps your iframe on top.
// The remainder of the line smacks the panel into the bottom left corner, out of your way.
// Overflow (in combination with the setTimeout) ensures the iframe fits your entire panel.
//var css = 'position:fixed; z-index:9999; bottom:0px; left:0px; border:0; margin:0; padding:0; ' +
//          'overflow:hidden;'
// Margin, top, left, width and height center the iframe horizontally and vertically:
//var css = 'position:fixed; z-index:9999; border:2px solid black; ' +
//          'top:50%; left:50%; width:50em; margin:15em 0 0 10em; height:40em;';
var css = 'position:fixed; z-index:9999; bottom:0px; left:0px; border:2px solid black; margin:0; padding:0; ' +
          'overflow:hidden;width:23em;height:12em;'

var iframe = document.createElement('iframe');
iframe.setAttribute('id', 'theIframe');
iframe.setAttribute('style', css);

// The about:blank page becomes a blank(!) canvas to modify
iframe.src = 'about:blank';

document.body.appendChild(iframe);

    var doc = iframe.contentDocument;
    doc.body.style.background = 'white';
//    doc.body.innerHTML = '<form action="equal.1" method="get" onsubmit="return parent.calc()"><input type=text name=a> = <input type=text name=total><input type=submit value="Calculate"></form>'
    doc.body.innerHTML = '<table><tr><center><b><u>Connection Manager</u></b> (<a href="javascript:parent.closeMe();">x</a>)</center></tr><tr><td><form action="connectReal.1" method="get" onsubmit="return parent.connectReal()"><button style="-webkit-appearance: none;" name="real" type="submit" value="toggleReal"><img id="realState" height="75" src="fig/realInit.jpg"></button></form></td><td><form action="connectSim.1" method="get" onsubmit="return parent.connectSim()"><button style="-webkit-appearance: none;" name="sim" type="submit" value="toggleSim"><img id="simState" height="75" src="fig/simInit.jpg"></button></form></td></tr></table>';

// Make sure Firefox initializes the DOM before we try to use it.
iframe.addEventListener("load", function() {
    var doc = iframe.contentDocument;
    doc.body.style.background = 'white';
//    doc.body.innerHTML = '<form action="equal.1" method="get" onsubmit="return parent.calc()"><input type=text name=a> = <input type=text name=total><input type=submit value="Calculate"></form>'
    doc.body.innerHTML = '<table><tr><center><b><u>Connection Manager</u></b> (<a href="javascript:parent.closeMe();">x</a>)</center></tr><tr><td><form action="connectReal.1" method="get" onsubmit="return parent.connectReal()"><button style="-webkit-appearance: none;" name="real" type="submit" value="toggleReal"><img id="realState" height="75" src="fig/realInit.jpg"></button></form></td><td><form action="connectSim.1" method="get" onsubmit="return parent.connectSim()"><button style="-webkit-appearance: none;" name="sim" type="submit" value="toggleSim"><img id="simState" height="75" src="fig/simInit.jpg"></button></form></td></tr></table>';
//    <input type=submit name=robot value="on"><br><input type=text name=status>

    // It seems Firefox (at least 3.6) has a bug. It will report offsetWidth less than clientWidth.
    // So try clientWidth and clientHeight instead of offsetWidth and offsetHeight
//    iframe.style.width = doc.body.offsetWidth + "px";
//    iframe.style.height = doc.body.offsetHeight + "px";
}, false);

function closeMe() {
    var ifr = parent.document.getElementById("theIframe");
    ifr.parentNode.removeChild(ifr);
    return false;
}

function connectReal() {
    var doc = iframe.contentDocument;
    frm=doc.forms[0];
    url="connectReal.1?real="+frm.elements['real'].value;
    xmlhttp.open("GET",url,true);
    xmlhttp.onreadystatechange=function() {
        if (xmlhttp.readyState==4) {
            if((xmlhttp.responseText.search('REALOFF'))>-1){
//                doc.body.style.background = 'green';
                if(!doc.getElementById('realState'))
                    alert('Not found realState');
                else
                    doc.getElementById('realState').src = 'fig/realDis.jpg';
                alert('Disconnected from Real Robot');
            } else {
                if(!doc.getElementById('realState'))
                    alert('Not found realState');
                else
                    doc.getElementById('realState').src = 'fig/realCon.jpg';
                alert('Connected to Real Robot');
            }
        }
    }
    xmlhttp.setRequestHeader('Accept','message/x-jl-formresult');
    xmlhttp.send();
    return false
}

function connectSim() {
    var doc = iframe.contentDocument;
    frm=doc.forms[1];
    url="connectSim.1?sim="+frm.elements['sim'].value;
    xmlhttp.open("GET",url,true);
    xmlhttp.onreadystatechange=function() {
        if (xmlhttp.readyState==4) {
            if((xmlhttp.responseText.search('SIMOFF'))>-1) {
                if(!doc.getElementById('simState'))
                    alert('Not found simState');
                else
                    doc.getElementById('simState').src = 'fig/simDis.jpg';
                alert('Disconnected from Simulator');
            } else {
                if(!doc.getElementById('simState'))
                    alert('Not found simState');
                else
                    doc.getElementById('simState').src = 'fig/simCon.jpg';
                alert('Connected to Simulator');
            }
        }
    }
    xmlhttp.setRequestHeader('Accept','message/x-jl-formresult');
    xmlhttp.send();
    return false
}

