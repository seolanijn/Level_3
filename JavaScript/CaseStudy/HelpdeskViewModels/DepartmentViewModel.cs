using HelpdeskDAL;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Reflection;
using System.Threading.Tasks;

namespace HelpdeskViewModels
{
    public class DepartmentViewModel
    {
        readonly private DepartmentDAO _dao;

        public int Id { get; set; }
        public string Name { get; set; }
        public string Timer { get; set; }

        // constructor
        public DepartmentViewModel()
        {
            _dao = new DepartmentDAO();
        }

        public async Task<List<DepartmentViewModel>> GetAll()
        {
            List<DepartmentViewModel> allVms = new List<DepartmentViewModel>();
            try
            {
                List<Department> allDepartments = await _dao.GetAll();
                foreach (Department div in allDepartments)
                {
                    DepartmentViewModel divVm = new DepartmentViewModel
                    {
                        Id = div.Id,
                        Name = div.DepartmentName,
                        Timer = Convert.ToBase64String(div.Timer)
                    };
                    allVms.Add(divVm);
                }
            }
            catch (Exception ex)
            {
                Debug.WriteLine("Problem in " + GetType().Name + " " +
                    MethodBase.GetCurrentMethod().Name + " " + ex.Message);
                throw;
            }
            return allVms;
        }
    }
}
