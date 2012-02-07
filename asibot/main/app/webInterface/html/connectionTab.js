
function calc() {
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
}

function connectRobot() {
  var doc = iframe.contentDocument;
  doc.body.style.background = 'red';
//  frm=document.forms[0]
  frm=doc.forms[0]
  url="connectRobot.1?target="+frm.elements['target'].value
  xmlhttp.open("GET",url,true);
  xmlhttp.onreadystatechange=function() {
   if (xmlhttp.readyState==4) {
//    document.forms[0].elements['status'].value=xmlhttp.responseText
    doc.forms[0].elements['status'].value=xmlhttp.responseText
   }
  }
 xmlhttp.setRequestHeader('Accept','message/x-jl-formresult')
 xmlhttp.send()
 return false
}

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
          'overflow:hidden;'

var iframe = document.createElement('iframe');
iframe.setAttribute('id', 'theIframe');
iframe.setAttribute('style', css);

// The about:blank page becomes a blank(!) canvas to modify
iframe.src = 'about:blank';

document.body.appendChild(iframe);

// Make sure Firefox initializes the DOM before we try to use it.
iframe.addEventListener("load", function() {
    var doc = iframe.contentDocument;
    doc.body.style.background = 'white';
//    doc.body.innerHTML = '<script src="xmlhttp.js" type="text/javascript"></script><script src="tabHelpers.js" type="text/javascript"></script><form id="cTab" action="connectRobot.1" method="get" onsubmit="alert2()"><input type=submit name=target value="on"><br><input type=text name=status></form>';
    doc.body.innerHTML = '<form action="equal.1" method="get" onsubmit="return parent.calc()"><input type=text name=a> = <input type=text name=total><input type=submit value="Calculate"></form>'

    // It seems Firefox (at least 3.6) has a bug. It will report offsetWidth less than clientWidth.
    // So try clientWidth and clientHeight instead of offsetWidth and offsetHeight
//    iframe.style.width = doc.body.offsetWidth + "px";
//    iframe.style.height = doc.body.offsetHeight + "px";
}, false);

