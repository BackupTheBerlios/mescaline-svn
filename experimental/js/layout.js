function layout(widgets){

	for (var i = 0; i < widgets.length; i++) {

		var currentWidget = document.getElementById(widgets[i]);
		currentWidget.style.position = "absolute";
		currentWidget.style.top = 31 + "px";

		if (i == 0) {
			
			currentWidget.style.left = 0 + "px";
		}
		else {
			
			var previousLeft = document.getElementById(widgets[i - 1]).offsetLeft;
			var previousWidth = document.getElementById(widgets[i - 1]).offsetWidth;
			var offsetLeft = previousLeft + previousWidth;
			currentWidget.style.left = offsetLeft + "px";
		}
	}
}

function place(id, x, y){

	var currentWidget = document.getElementById(id);
	currentWidget.style.position = "absolute";
	currentWidget.style.top = y + "px";
	currentWidget.style.left = x + "px";
}

function makeMoveable(sessionString, widgets){

 	for (var i = 0; i < widgets.length; i++) {
 
 		var widget = new dojo.dnd.Moveable(widgets[i], {handle: widgets[i] + "_handle"});

 		dojo.connect(widget, "onMoveStop", function(mover){

			var pos = dojo.coords(mover.node);

			var x = mover.node.style.left.replace("px", "");
			var y = mover.node.style.top.replace("px", "");
			var width = mover.node.offsetWidth;

			// 	Ensure that the handle does not move out of the browser window.

			if (y < 0) y = 0;
			if (x < (0 - width + 20)) x = 20 - width;

			window.location.replace("./index.php?" + sessionString + "&setPosition=" + mover.node.id + "," + x + "x" + y);
		});
	}
}

