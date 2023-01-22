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
    public class StudentController : ControllerBase
    {
        [HttpGet("{lastname}")]
        public async Task<IActionResult> GetByLastname(string lastname)
        {
            try
            {
                StudentViewModel viewmodel = new StudentViewModel
                {
                    Lastname = lastname
                };
                await viewmodel.GetByLastname();
                return Ok(viewmodel);
            }
            catch (Exception ex)
            {
                Debug.WriteLine("Problem in " + GetType().Name + " " +
                    MethodBase.GetCurrentMethod().Name + " " + ex.Message);
                return StatusCode(StatusCodes.Status500InternalServerError);    // something went wrong
            }
        }

        [HttpPut]
        public async Task<ActionResult> Put(StudentViewModel viewmodel)
        {
            try
            {
                int retVal = await viewmodel.Update();
                return retVal switch
                {
                    1 => Ok(new { msg = "Student " + viewmodel.Lastname + " updated!" }),
                    -1 => Ok(new { msg = "Student " + viewmodel.Lastname + " not updated!" }),
                    -2 => Ok(new { msg = "Data is stale for " + viewmodel.Lastname + ", Student not updated!" }),
                    _ => Ok(new { msg = "Student " + viewmodel.Lastname + " not updated!" }),
                };
            }
            catch (Exception ex)
            {
                Debug.WriteLine("Problem in " + GetType().Name + " " +
                    MethodBase.GetCurrentMethod().Name + " " + ex.Message);
                return StatusCode(StatusCodes.Status500InternalServerError);    // something went wrong
            }
        }

        [HttpGet]
        public async Task<IActionResult> GetAll()
        {
            try
            {
                StudentViewModel viewmodel = new StudentViewModel();
                List<StudentViewModel> allStudents = await viewmodel.GetAll();
                return Ok(allStudents);
            }
            catch (Exception ex)
            {
                Debug.WriteLine("Problem in " + GetType().Name + " " +
                    MethodBase.GetCurrentMethod().Name + " " + ex.Message);
                return StatusCode(StatusCodes.Status500InternalServerError);    // something went wrong
            }
        }

        [HttpPost]
        public async Task<ActionResult> Post(StudentViewModel viewmodel)
        {
            try
            {
                await viewmodel.Add();
                return viewmodel.Id > 1
                    ? Ok(new { msg = "Student " + viewmodel.Lastname + " added!" })
                    : Ok(new { msg = "Student " + viewmodel.Lastname + " not added!" });
            }
            catch (Exception ex)
            {
                Debug.WriteLine("Problem in " + GetType().Name + " " +
                    MethodBase.GetCurrentMethod().Name + " " + ex.Message);
                return StatusCode(StatusCodes.Status500InternalServerError);    // something went wrong
            }
        }

        [HttpDelete("{id}")]
        public async Task<IActionResult> Delete(int id)
        {
            try
            {
                StudentViewModel viewmodel = new StudentViewModel { Id = id };
                return await viewmodel.Delete() == 1
                    ? Ok(new { msg = "Student " + id + " deleted!" })
                    : Ok(new { msg = "Student " + id + " not deleted!" });
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
