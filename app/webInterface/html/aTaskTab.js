
// This iframe stuff is from http://wiki.greasespot.net/CSS_Independent_Content [thanks!]

// position:fixed means stay fixed even when the page scrolls. z-index keeps your iframe on top.
// The remainder of the line smacks the panel into the bottom left corner, out of your way.
// Overflow (in combination with the setTimeout) ensures the iframe fits your entire panel.
//var css = 'position:fixed; z-index:9999; bottom:0px; left:0px; border:0; margin:0; padding:0; ' +
//          'overflow:hidden;'
// Margin, top, left, width and height center the iframe horizontally and vertically:
//var css = 'position:fixed; z-index:9999; border:2px solid black; ' +
//          'top:50%; left:50%; width:50em; margin:15em 0 0 10em; height:40em;';

var frm2 = document.createElement('form');
frm2.setAttribute('id', 'frm2');
frm2.setAttribute('name', 'frm2');
frm2.setAttribute('action', 'launcher');
document.body.appendChild(frm2);

var css2 = 'position:fixed; z-index:5000; bottom:0px; right:0px; border:2px solid black; margin:0; padding:0; ' +
          'overflow:hidden;width:23em;height:8em;'

var iframe2 = document.createElement('iframe');
iframe2.setAttribute('id', 'theIframe2');
iframe2.setAttribute('style', css2);

// The about:blank page becomes a blank(!) canvas to modify
iframe2.src = 'about:blank';

document.body.appendChild(iframe2);

    var doc = iframe2.contentDocument;
    doc.body.style.background = 'white';
    doc.body.innerHTML = '<table><tr><center><b><u>Task Creator Progress</u></b> (<a href="javascript:parent.closeMe2();">x</a>)</center></tr><tr><td><button type="submit" onClick="return htask(this.form)"><img src="fig/ballBlacks.png" width="30"alt=""></button><button type="submit" onClick="return htask(this.form)"><img src="fig/ballBlacks.png" width="30"alt=""></button><button type="submit" onClick="return htask(this.form)"><img src="fig/ballBlacks.png" width="30"alt=""></button><button type="submit" onClick="return htask(this.form)"><img src="fig/ballBlacks.png" width="30"alt=""></button><button type="submit" onClick="return htask(this.form)"><img src="fig/ballBlues.png" width="30"alt=""></button></td></tr><tr><td align=center><a href="javascript:parent.nextTab();">Finished!!Go to Launcher \>\></a></td></tr></table>';

// Make sure Firefox initializes the DOM before we try to use it.
iframe2.addEventListener("load", function() {
    var doc = iframe2.contentDocument;
    doc.body.style.background = 'white';
    doc.body.innerHTML = '<table><tr><center><b><u>Task Creator Progress</u></b> (<a href="javascript:parent.closeMe2();">x</a>)</center></tr><tr><td><button type="submit" onClick="return htask(this.form)"><img src="fig/ballBlacks.png" width="30"alt=""></button><button type="submit" onClick="return htask(this.form)"><img src="fig/ballBlacks.png" width="30"alt=""></button><button type="submit" onClick="return htask(this.form)"><img src="fig/ballBlacks.png" width="30"alt=""></button><button type="submit" onClick="return htask(this.form)"><img src="fig/ballBlacks.png" width="30"alt=""></button><button type="submit" onClick="return htask(this.form)"><img src="fig/ballBlues.png" width="30"alt=""></button></td></tr><tr><td align=center><a href="javascript:parent.nextTab();">Finished!! Go to Launcher \>\></a></td></tr></table>';
//    <input type=submit name=robot value="on"><br><input type=text name=status>

    // It seems Firefox (at least 3.6) has a bug. It will report offsetWidth less than clientWidth.
    // So try clientWidth and clientHeight instead of offsetWidth and offsetHeight
//    iframe.style.width = doc.body.offsetWidth + "px";
//    iframe.style.height = doc.body.offsetHeight + "px";
}, false);

function closeMe2() {
    var ifr2 = parent.document.getElementById("theIframe2");
    ifr2.parentNode.removeChild(ifr2);
    return false;
}

function nextTab() {
    if(!confirm('This was the final step. Are you sure you have finished?\n\n\nClick on OK to close the Task Creator and go to the Launcher Tab...')) return false;
    var ifr3 = parent.document.getElementById("frm2");
    ifr3.submit();
    return false;
}

