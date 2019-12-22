Page({
  data: {
    device_name: 32,

  },
//调用onenet api获取从arduino获取的数据值
  OnLoad: function (options) {
    var _this = this;//用onLoad函数直接刷新当前垃圾桶的垃圾深度
    //console.log(options.device_id);// 打印设备ID
    //console.log(app.data.api_key);// 打印API_KEY
    wx.request({
      url: 'https://api.heclouds.com/devices/562234373/datapoints',
      header: {
        'api-key': 'Rh3VomUuIy1yXEpJURmUlx7npRE='
      },
      success: function (res) {
        console.log(res.data);//在控制台上打印
        _this.setData(
          {
            device_name: res.data.data.datastreams[0].datapoints[0].value//获取数据值后将其传送到data

          });
      },
      fail: function () {
        wx.showToast({
          title: '与服务器通信失败',
          icon: 'fail',
          duration: 2000
        })//与服务器无法连接时提醒
      }
    })
  },

})
