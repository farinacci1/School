$(document).on('click',".navBarOption", function(event){
	$(".navBarOption").removeClass('active');
    $(this).toggleClass('active');
});