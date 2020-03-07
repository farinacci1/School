$(document).ready(function() {
	$("#createHospital").click(function(e){
		$("#HospitalModal").css("display", "block");
		if($('#HospitalModal').css('display') == 'block')
		{
			$("#Modalclose").click(function(e){
				$("#HospitalModal").css("display", "none");
			});
			$(window).click(function(e) {
				if(e.target.id == $('#HospitalModal')[0].id){
					 $("#HospitalModal").css("display", "none");
				}
				
			});
		}
	});


});