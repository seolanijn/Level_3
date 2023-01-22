using ExercisesViewModels;
using Microsoft.AspNetCore.Http;
using Microsoft.AspNetCore.Mvc;
using System;
using System.Diagnostics;
using System.Reflection;
using System.Linq;
using System.Threading.Tasks;
using System.Collections.Generic;

namespace ExercisesWebsite.Controllers
{
    [Route("api/[controller]")]
    [ApiController]
    public class DivisionController : ControllerBase
    {
        [HttpGet]
        public async Task<IActionResult> GetAll()
        {
            try
            {
                DivisionViewModel viewmodel = new DivisionViewModel();
                List<DivisionViewModel> allDivisions = await viewmodel.GetAll();
                return Ok(allDivisions);
            }
            catch (Exception ex)
            {
                Debug.WriteLine("Problem in " + GetType().Name + " " +
                    MethodBase.GetCurrentMethod().Name + " " + ex.Message);
                return StatusCode(StatusCodes.Status500InternalServerError);    // something went wrong
            }
        }
    }
}
