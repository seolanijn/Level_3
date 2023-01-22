/* filename: EmployeeController.cs
 * description: it will help to expose viewmodel data to the outside
 */
using HelpdeskViewModels;
using Microsoft.AspNetCore.Http;
using Microsoft.AspNetCore.Mvc;
using System;
using System.Diagnostics;
using System.Reflection;
using System.Linq;
using System.Threading.Tasks;
using System.Collections.Generic;

namespace CasestudyWebsite.Controllers
{
    [Route("api/[controller]")]
    [ApiController]
    public class EmployeeController : ControllerBase
    {
        // get employee by email
        [HttpGet("{Email}")]
        public async Task<IActionResult> GetByEmail(string email)
        {
            try
            {
                EmployeeViewModel viewmodel = new EmployeeViewModel
                {
                    Email = email
                };
                await viewmodel.GetByEmail();
                return Ok(viewmodel);
            }
            catch (Exception ex)
            {
                Debug.WriteLine("Problem in " + GetType().Name + " " +
                    MethodBase.GetCurrentMethod().Name + " " + ex.Message);
                return StatusCode(StatusCodes.Status500InternalServerError);    // something went wrong
            }
        }

        // update employee and print the current statement on the screen
        [HttpPut]
        public async Task<ActionResult> Put(EmployeeViewModel viewmodel)
        {
            try
            {
                int retVal = await viewmodel.Update();
                return retVal switch
                {
                    1 => Ok(new { msg = "Employee " + viewmodel.Lastname + " updated!" }),
                    -1 => Ok(new { msg = "Employee " + viewmodel.Lastname + " not updated!" }),
                    -2 => Ok(new { msg = "Data is stale for " + viewmodel.Lastname + ", Employee not updated!" }),
                    _ => Ok(new { msg = "Employee " + viewmodel.Lastname + " not updated!" }),
                };
            }
            catch (Exception ex)
            {
                Debug.WriteLine("Problem in " + GetType().Name + " " +
                    MethodBase.GetCurrentMethod().Name + " " + ex.Message);
                return StatusCode(StatusCodes.Status500InternalServerError);    // something went wrong
            }
        }

        // get all employees
        [HttpGet]
        public async Task<IActionResult> GetAll()
        {
            try
            {
                EmployeeViewModel viewmodel = new EmployeeViewModel();
                List<EmployeeViewModel> allEmployees = await viewmodel.GetAll();
                return Ok(allEmployees);
            }
            catch (Exception ex)
            {
                Debug.WriteLine("Problem in " + GetType().Name + " " +
                    MethodBase.GetCurrentMethod().Name + " " + ex.Message);
                return StatusCode(StatusCodes.Status500InternalServerError);    // something went wrong
            }
        }

        // add employee and print the current statement on the screen
        [HttpPost]
        public async Task<ActionResult> Post(EmployeeViewModel viewmodel)
        {
            try
            {
                await viewmodel.Add();
                return viewmodel.Id > 1
                    ? Ok(new { msg = "Employee " + viewmodel.Lastname + " added!" })
                    : Ok(new { msg = "Employee " + viewmodel.Lastname + " not added!" });
            }
            catch (Exception ex)
            {
                Debug.WriteLine("Problem in " + GetType().Name + " " +
                    MethodBase.GetCurrentMethod().Name + " " + ex.Message);
                return StatusCode(StatusCodes.Status500InternalServerError);    // something went wrong
            }
        }

        // add employee and print the current statement on the screen
        [HttpDelete("{id}")]
        public async Task<IActionResult> Delete(int id)
        {
            try
            {
                EmployeeViewModel viewmodel = new EmployeeViewModel { Id = id };
                return await viewmodel.Delete() == 1
                    ? Ok(new { msg = "Employee " + id + " deleted!" })
                    : Ok(new { msg = "Employee " + id + " not deleted!" });
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
